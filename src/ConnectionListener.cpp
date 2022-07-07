#include "ConnectionListener.hpp"
#include "Connection.hpp"
#include <iostream>

ConnectionListener::ConnectionListener(int port, int ai_socktype): 
    _ad(), 
    _mySock(),
    _closed(false)
{
    memset(&_ad, 0, sizeof(_ad));

    constexpr int serviceProviderChooseProtocolForMe = 0;

    _ad.ai_addrlen = INET6_ADDRSTRLEN;
    _ad.ai_family = AF_INET6;
    _ad.ai_socktype=ai_socktype;
    _ad.ai_protocol=serviceProviderChooseProtocolForMe;
    _ad.ai_addr = reinterpret_cast<sockaddr*>(&_ss);
    
    memset(&_ss, 0, sizeof(_ss));
    auto ss = reinterpret_cast<sockaddr_in6*>(&_ss);
    ss->sin6_addr = in6addr_any;
    ss->sin6_family = AF_INET6;
    ss->sin6_port = htons(port);
    //ss->sin6_addr = htonl(INADDR_ANY); ?? It worked in IPv4, now it does not.
    //inet_pton(AF_INET6, "::1", &(ss->sin_addr.s_addr));
    //inet_pton(AF_INET, "127.0.0.1", &(ss->sin_addr.s_addr));

    addrinfo* adList = nullptr;

    int status = 0;

    status = getaddrinfo(NULL, std::to_string(port).c_str(), &_ad, &adList);

    if (status != 0)
        ThrowErrnoMsg("ConnectionListener failed to getaddrinfo: ");

    // TODO: Traverse "result" list for valid entry?
    // Here I'm assuming the first entry is always good
    _mySock = socket(adList->ai_family, 
                    adList->ai_socktype, 
                    adList->ai_protocol);
                
    //std::cout<<"ConnectionListener new socket: "<<_mySock<<"\n";

    if (_mySock == -1)
        ThrowErrnoMsg("ConnectionListener failed to create a socket: ");
    
    status = bind(_mySock, adList->ai_addr, adList->ai_addrlen);

    if (status == -1)
        ThrowErrnoMsg("ConnectionListener failed to bind socket: ");

    freeaddrinfo(adList);
}

ConnectionListener::~ConnectionListener()
{
    Close();
}

int ConnectionListener::GetPort()
{
    return reinterpret_cast<sockaddr_in6*>(&_ss)->sin6_port;
}

void ConnectionListener::Listen()
{
    //std::cout<<"Listening: "<<listen(_mySock, 20)<<"\n";
    listen(_mySock, 20);
}

std::shared_ptr<Connection> ConnectionListener::Accept()
{
    sockaddr_storage newSA;
    socklen_t addr_size = sizeof(newSA);
    int newSock = accept(_mySock, (sockaddr*) &newSA, &addr_size);
    //std::cout<<"Accepted new socket: "<<newSock<<"\n";
    if (newSock == -1)
    {
        ThrowErrnoMsg("ConnectionListener bad accept: ");
    }
    auto newConnection = std::shared_ptr<Connection>(new Connection(newSock, newSA, GetSockType()));
    return newConnection;
}

int ConnectionListener::GetSockType()
{
    return _ad.ai_socktype; 
}

void ConnectionListener::Close()
{
    if (!_closed)
    {
        _closed = true;
        close(_mySock);
    }
}
