#include "ConnectionListener.hpp"
#include "Talker.hpp"
#include <iostream>

ConnectionListener::ConnectionListener(int port, int ai_socktype): _ad(), _sin6(), _adList(nullptr), _mySock()
{
    // Setup Sockaddr_in6
    _sin6.sin6_family = AF_INET6;
    inet_pton(AF_INET6, IPv6_Loopback, &(_sin6.sin6_addr));
    _sin6.sin6_port = port;
    _ad.ai_addr = reinterpret_cast<sockaddr*>(&_sin6);

    _ad.ai_family = AF_INET6;
    _ad.ai_socktype = ai_socktype;

    // TODO: Check returned error code 
    getaddrinfo(IPv6_Loopback, std::to_string(_sin6.sin6_port).c_str(), &_ad, &_adList);

    // TODO: Traverse "result" list for valid entry?
    // Here I'm assuming the first entry is always good
    _mySock = socket(_adList->ai_family, _adList->ai_socktype, _adList->ai_protocol);
}

ConnectionListener::~ConnectionListener()
{
    if (_adList != nullptr)
        freeaddrinfo(_adList);
}

void ConnectionListener::Listen()
{
    // TODO: Check returned error code 
    bind(_mySock, _adList->ai_addr, _adList->ai_addrlen);
    std::cout<<"Listening: "<<listen(_mySock, 20)<<"\n";
}

Talker* ConnectionListener::Accept()
{
    sockaddr_storage newSA;
    socklen_t addr_size = sizeof(newSA);

    int newSock = accept(_mySock, (sockaddr*) &newSA, &addr_size);
    return nullptr;
}