#include "Connection.hpp"
#include <iostream>
// Try to connect to ip:port destination
Connection::Connection(const std::string &ip, int port, int ai_socktype): _sockType(ai_socktype)
{
    auto result = GetAddrInfo(ip, port, ai_socktype);

    // Pick the first result to use
    _remoteInfo = *reinterpret_cast<sockaddr_storage*>(result->ai_addr);

    // TODO: Walk the list for valid entry
    _sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    //std::cout<<"Socket call: "<<_sock<<"\n";

    // TODO: Check returned error code 
    // std::cout<<"Connect: "<<connect(_sock, result->ai_addr, result->ai_addrlen)<<"\n";
    connect(_sock, result->ai_addr, result->ai_addrlen);

    freeaddrinfo(result);
}

Connection::Connection(int acceptedSocket, sockaddr_storage sockInfo, int sockType):
    _remoteInfo(sockInfo),
    _sock(acceptedSocket),
    _sockType(sockType)
{
}

addrinfo* Connection::GetAddrInfo(const std::string &ip, int port, int ai_socktype)
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

Connection::~Connection()
{
    close(_sock);
}

void Connection::Send(const std::string& message)
{
    long long messageSize = message.length()*sizeof(message[0]);
    long long sentSize(0);

    std::string_view s(message);

    while (sentSize < messageSize)
    {
        int bytes_sent = send(_sock, s.begin(), s.size() , 0);
        if (bytes_sent == -1)
            throw std::runtime_error("Connection failed to send message. Check errno (in <errno.h>) for what happened.");

        sentSize += bytes_sent;
        s.substr(bytes_sent);
    }
}

std::string Connection::Receive()
{
    int bytes_read = recv(_sock, _recvBuffer, sizeof(_recvBuffer), 0);
    if (bytes_read == -1)
        throw std::runtime_error("Connection failed to receive message. See @errno (in <errno.h>) for more details.");

    return std::string(_recvBuffer).substr(0, bytes_read);
}

std::string Connection::GetRemoteIP()
{
    char ip[INET6_ADDRSTRLEN];
    const void *ipField;

    if (_remoteInfo.ss_family == AF_INET6) 
        ipField = &(reinterpret_cast<sockaddr_in6*>(&_remoteInfo)->sin6_addr);
    else 
        ipField = &(reinterpret_cast<sockaddr_in*>(&_remoteInfo)->sin_addr);

    return inet_ntop(_remoteInfo.ss_family, ipField, ip, INET6_ADDRSTRLEN);
}

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

bool Connection::IsIPv6QuickCheck(const std::string &ip)
{
    return ip.find(':') != std::string::npos;
}