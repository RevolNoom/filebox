#ifndef CONNECTION_LISTENER_HPP
#define CONNECTION_LISTENER_HPP

#include <string>
#include <memory>

#include "Socket.hpp"

class Talker;

class ConnectionListener
{
public:

    // @port: Port that this socket will listen to
    // @ai_socktype: SOCK_DGRAM for UDP, SOCK_STREAM for TCP
    ConnectionListener(int port, int ai_socktype);

    ~ConnectionListener();

    int GetPort();

    void Listen();
    Talker* Accept();
    void Close();

private:
    #define IPv6_Loopback  "::1"

private:
    addrinfo _ad;
    sockaddr_in6 _sin6;
    addrinfo* _adList;
    int _mySock;
};

#endif /* CONNECTION_LISTENER_HPP */