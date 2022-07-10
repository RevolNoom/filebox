#include "Connection.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include "BackgroundConnectionUpdater.hpp"
#include <fcntl.h>

void ThrowErrnoMsg(const std::string &msg)
{
    throw std::runtime_error(msg + std::string(strerror(errno)));
}

// Try to connect to ip:port destination
Connection::Connection(const std::string &ip, int port, int ai_socktype): 
    _sockType(ai_socktype),
    _closed(false)
{
    auto result = GetAddrInfo(ip, port, ai_socktype);

    // Pick the first result to use
    _remoteInfo = *reinterpret_cast<sockaddr_storage*>(result->ai_addr);

    // TODO: Walk the list for valid entry
    _sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    //std::cout<<"Socket call: "<<_sock<<"\n";
    if (_sock == -1)
        ThrowErrnoMsg("Connection failed to create new socket: ");

    //std::cout<<"Connection new socket: "<<_sock<<"\n";

    if (connect(_sock, result->ai_addr, result->ai_addrlen) == -1)
        ThrowErrnoMsg("Connection failed: ");
    //    _state = ERROR;
    //}
    //else
    //    _state = CONNECTED;

    freeaddrinfo(result);

    // TODO: Maybe refactor these two lines?
    fcntl(_sock, F_SETFL, O_NONBLOCK);
    BackgroundConnectionUpdater::Singleton().Subscribe(this);
}

Connection::Connection(int acceptedSocket, sockaddr_storage sockInfo, int sockType):
    _remoteInfo(sockInfo),
    _sock(acceptedSocket),
    _sockType(sockType),
    _closed(false)
{
    if (_sock == -1)
        ThrowErrnoMsg("Connection failed to create new socket: ");
    fcntl(_sock, F_SETFL, O_NONBLOCK);
    BackgroundConnectionUpdater::Singleton().Subscribe(this);
}

Connection::~Connection()
{
    Close();
}


// A packet has the following format:
// [byte-size] message ...
void Connection::Send(const std::string& message)
{
    if (IsClosed())
        ThrowErrnoMsg("Connection closed.");

    Int64ToRawBytes messageSize;
    messageSize._int = message.length();

    // The actual sent message is appended to the head with packet size
    std::string sendmsg = std::string(messageSize._str, MESSAGE_HEADER_SIZE) + message;

    //std::cout<<"Sending message "<<messageSize._int<<" chars: "<<message<<"\n";

    std::string_view s(sendmsg);

    while (s.length() > 0)
    {
        int packetSize = std::min(s.length(), MAX_BUFFER_SIZE-1);
        int bytes_sent = send(_sock, s.begin(), packetSize, 0);
        //std::cout<<"Bytes sent: "<<bytes_sent<<"\n";
        if (bytes_sent == -1)
        {
            ThrowErrnoMsg("Connection failed to send message: ");
            Close();
        }

        s.remove_prefix(bytes_sent);
    }
}

void Connection::Close() 
{
    if (!IsClosed())
    {
        _closed = true;
        BackgroundConnectionUpdater::Singleton().Unsubscribe(this);
        close(_sock);
    }
}

bool Connection::IsClosed() const
{
    return _closed;
}

std::string Connection::Receive(bool blocking)
{
    if (IsClosed())
        ThrowErrnoMsg("Socket closed: " + std::to_string(_sock));

    std::string msg;
    do
    {
        msg = _recvBuffer.GetMessage();

        if (msg != "")
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //std::cout<<"Blocking!\n";
    }
    while (blocking);

    //if (msg.length())
    //    std::cout<<"Received: '"<<msg<<"'\n";

    return msg;
}

void Connection::FetchBuffer()
{
    if (IsClosed())
        ThrowErrnoMsg("Socket closed: " + std::to_string(_sock));

    //std::cout<<"Fetching buffer for "<<_sock<<"\n";
    char recvBuffer[MAX_BUFFER_SIZE];

    constexpr bool recvHasSomething = true;
    while (recvHasSomething)
    {
        int bytes_read = recv(_sock, recvBuffer, MAX_BUFFER_SIZE-1, MSG_DONTWAIT);
        if (bytes_read == -1)
        {
            // F Unix. Why you block me even when select() says recv() is ready? =_=
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                //std::cout<<"Would block\n";
                return;
            }

            Close();
            ThrowErrnoMsg("Connection failed to receive message because of: ");
        }

        if (bytes_read == 0)
        {
            Close();
            return;
        }

        _recvBuffer.Write(recvBuffer, bytes_read);
        break;
    }
}

std::string Connection::GetRemoteIP() const
{
    char ip[INET6_ADDRSTRLEN];
    const void *ipField;

    if (_remoteInfo.ss_family == AF_INET6) 
        ipField = &(reinterpret_cast<const sockaddr_in6*>(&_remoteInfo)->sin6_addr);
    else 
        ipField = &(reinterpret_cast<const sockaddr_in*>(&_remoteInfo)->sin_addr);

    return inet_ntop(_remoteInfo.ss_family, ipField, ip, INET6_ADDRSTRLEN);
}
/*
int Connection::GetPort()
{
    if (_remoteInfo.ss_family == AF_INET6) 
        return static_cast<int>(reinterpret_cast<sockaddr_in6*>(&_remoteInfo)->sin6_port);
    return static_cast<int>(reinterpret_cast<sockaddr_in*>(&_remoteInfo)->sin_port);
}


int Connection::GetSockType()
{
    return _sockType; 
}
*/

bool Connection::IsIPv6QuickCheck(const std::string &ip) const
{
    return ip.find(':') != std::string::npos;
}

addrinfo* Connection::GetAddrInfo(const std::string &ip, int port, int ai_socktype) const
{
    addrinfo ad;
    memset(&ad, 0, sizeof(ad));

    sockaddr_storage ss;

    if (IsIPv6QuickCheck(ip))
    {
        sockaddr_in6 s6;
        memset(&s6, 0, sizeof(s6));
        s6.sin6_family = AF_INET6;
        s6.sin6_port = port;
        ss = *reinterpret_cast<sockaddr_storage*>(&s6);
    }
    else
    {
        sockaddr_in s4;
        memset(&s4, 0, sizeof(s4));
        s4.sin_family = AF_INET;
        s4.sin_port = port;
        ss = *reinterpret_cast<sockaddr_storage*>(&s4);
    }

    ad.ai_addr = reinterpret_cast<sockaddr*>(&ss);
    ad.ai_family = ss.ss_family;
    ad.ai_socktype = ai_socktype;

    addrinfo* result;

    // TODO: Check returned error code 
    //std::cout<<"Get addrinfo: "<<getaddrinfo(ip.c_str(), std::to_string(sin6.sin6_port).c_str(), &ad, &result)<<"\n";
    getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &ad, &result);

    return result;
}


void Connection::Buffer::Write(const char* src, int count)
{
    _bufferProtector.lock();
    //std::cout<<"Writing: '";
    for (int iii=0; iii<count; ++iii)
    {
    //    std::cout<<char(src[iii])<<"";
        _recvBuffer.push_back(src[iii]);
    }
    //std::cout<<"'\n";
    _bufferProtector.unlock();
}


std::string Connection::Buffer::GetMessage()
{
    // Avoid reading out-of-range stream
    if (_recvBuffer.size() < MESSAGE_HEADER_SIZE)
    {
        return "";
    }

    Int64ToRawBytes sizeStamp;
    strncpy(sizeStamp._str, GetFront(MESSAGE_HEADER_SIZE).c_str(), MESSAGE_HEADER_SIZE);

    if (sizeStamp._int > _recvBuffer.size())
    {
        WriteFront(sizeStamp._str, MESSAGE_HEADER_SIZE);
        return "";
    }

    return GetFront(sizeStamp._int);
}

void Connection::Buffer::WriteFront(const char* src, int count)
{
    _bufferProtector.lock();
    while (count-- >= 0)
        _recvBuffer.push_front(src[count]);
    _bufferProtector.unlock();
}


std::string Connection::Buffer::GetFront(int count)
{
    std::string result;
    result.reserve(count);

    _bufferProtector.lock();
    for (int iii = 0; iii<count; ++iii)
    {
        result+=_recvBuffer.front();
        _recvBuffer.pop_front();
    }
    _bufferProtector.unlock();

    return result;
}

int Connection::GetFileDescriptor() const
{
    return _sock;
}