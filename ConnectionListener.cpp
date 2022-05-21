#include "ConnectionListener.hpp"
#include "Talker.hpp"
#include <iostream>

ConnectionListener::ConnectionListener(int port, int ai_socktype): _ad(), _mySock()
{
    memset(&_ad, 0, sizeof(_ad));

    int serviceProviderChooseProtocolForMe = 0;

    //_ad.ai_addrlen = INET6_ADDRSTRLEN;
    _ad.ai_family = AF_UNSPEC;
    _ad.ai_socktype=SOCK_STREAM;
    _ad.ai_flags = AI_PASSIVE;
    _ad.ai_protocol=serviceProviderChooseProtocolForMe;
    _ad.ai_addr = reinterpret_cast<sockaddr*>(&_ss);
    
    memset(&_ss, 0, sizeof(_ss));
    auto ss = reinterpret_cast<sockaddr_in*>(&_ss);
    ss->sin_family = AF_INET;
    ss->sin_port = htons(port);
    ss->sin_addr.s_addr = htonl(INADDR_ANY);

    //ad.ai_family = AF_UNSPEC; // v4 or v6 are both fineeee
    //ad.ai_flags = AI_PASSIVE; // Fill my IP for me
    //ad.ai_socktype = ai_socktype;

    addrinfo* adList = nullptr;

    int status = 0;

    status = getaddrinfo(NULL, std::to_string(port).c_str(), &_ad, &adList);

    if (status != 0)
        throw std::runtime_error("ConnectionListener failed to getaddrinfo.");

    // TODO: Traverse "result" list for valid entry?
    // Here I'm assuming the first entry is always good
    //_mySock = socket(_adList->ai_family, _adList->ai_socktype, _adList->ai_protocol);
    _mySock = socket(adList->ai_family, 
                    adList->ai_socktype, 
                    adList->ai_protocol);
                
    if (_mySock == -1)
        throw std::runtime_error("ConnectionListener failed to create a socket.");

    _closed = false;
    
    status = bind(_mySock, adList->ai_addr, adList->ai_addrlen);

    if (status == -1)
        throw std::runtime_error("ConnectionListener failed to bind socket.");

    freeaddrinfo(adList);
}

ConnectionListener::~ConnectionListener()
{
    Close();
    //freeaddrinfo(_adList);
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
    return _ad.ai_socktype; //_adList->ai_socktype;
}

void ConnectionListener::Close()
{
    if (!_closed)
    {
        _closed = true;
        #ifdef _WIN32
            closesocket(_mySock);
        #else
            close(_mySock);
        #endif
    }
}