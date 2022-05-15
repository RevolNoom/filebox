#include "ConnectionListener.hpp"
#include "Talker.hpp"
#include <iostream>

ConnectionListener::ConnectionListener(int port, int ai_socktype): _adList(nullptr), _mySock()
{
    addrinfo ad;

    memset(&ad, 0, sizeof(ad));
    ad.ai_family = AF_UNSPEC; // v4 or v6 are both fineeee
    ad.ai_flags = AI_PASSIVE; // Fill my IP for me
    ad.ai_socktype = ai_socktype;

    // TODO: Check returned error code 
    getaddrinfo(nullptr, std::to_string(port).c_str(), &ad, &_adList);

    // TODO: Traverse "result" list for valid entry?
    // Here I'm assuming the first entry is always good
    _mySock = socket(_adList->ai_family, _adList->ai_socktype, _adList->ai_protocol);

    _closed = false;

    // TODO: Check returned error code 
    bind(_mySock, _adList->ai_addr, _adList->ai_addrlen);
}

ConnectionListener::~ConnectionListener()
{
    Close();
    freeaddrinfo(_adList);
}

void ConnectionListener::Listen()
{
    //std::cout<<"Listening: "<<listen(_mySock, 20)<<"\n";
    listen(_mySock, 20);
}

Talker ConnectionListener::Accept()
{
    sockaddr_storage newSA;
    socklen_t addr_size = sizeof(newSA);
    int newSock = accept(_mySock, (sockaddr*) &newSA, &addr_size);
    return Talker(newSock, newSA, GetSockType());
}

int ConnectionListener::GetSockType()
{
    return _adList->ai_socktype;
}

void ConnectionListener::Close()
{
    if (!_closed)
    {
        _closed = true;
        close(_mySock);
    }
}