#include "Talker.hpp"
#include <iostream>
#include <string.h>
#include <errno.h>

// Try to connect to ip:port destination
Talker::Talker(const std::string &ip, int port, int ai_socktype)
{
    // Setup Sockaddr_in6
    sockaddr_in6 sin6;
    sin6.sin6_family = AF_INET6;
    std::cout<<"Jamming IP down sockaddr throat: "<< inet_pton(AF_INET6, ip.c_str(), &(sin6.sin6_addr))<<"\n";
    sin6.sin6_port = port;

    std::cout<<GetRemoteIP()<<"\n";;

    addrinfo ad;
    memset(&ad, 0, sizeof(ad));
    ad.ai_addr = reinterpret_cast<sockaddr*>(&sin6);
    ad.ai_family = AF_INET6;
    ad.ai_socktype = ai_socktype;

    addrinfo* result;

    // TODO: Check returned error code 
    std::cout<<"IP: "<<ip.c_str()<<"\n";
    std::cout<<"Port: "<<std::to_string(sin6.sin6_port).c_str()<<"\n";
    
    std::cout<<"Get addrinfo: "<<getaddrinfo(ip.c_str(), std::to_string(sin6.sin6_port).c_str(), &ad, &result)<<"\n";
    fprintf(stderr, "%s", hstrerror( errno ));

    std::cout<<"Result pointer: "<<result<<"\n";
    // TODO: Walk the list for valid entry
    _sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    std::cout<<"Socket call: "<<_sock<<"\n";

    // TODO: Check returned error code 
    std::cout<<"Connect: "<<connect(_sock, result->ai_addr, result->ai_addrlen)<<"\n";

//    freeaddrinfo(result);
}

Talker::Talker(int acceptedSocket, sockaddr_storage sockInfo)
{
    _remoteInfo = sockInfo;
    _sock = acceptedSocket;
}

void Talker::Send(const std::string& message)
{
    // TODO: Loop until the message is sent
    send(_sock, message.c_str(), message.length()*sizeof(message[0]), 0);
}

std::string Talker::Receive()
{
    recv(_sock, _recvBuffer, sizeof(_recvBuffer), 0);
    return std::string(_recvBuffer);
}

std::string Talker::GetRemoteIP()
{
    char ip[INET6_ADDRSTRLEN];
    inet_ntop(_remoteInfo.ss_family, &(_remoteInfo), ip, INET6_ADDRSTRLEN);
    return ip;
}

int Talker::GetSockType()
{
    return 0; //_remoteInfo->
}