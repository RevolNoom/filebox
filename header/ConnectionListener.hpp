#ifndef CONNECTION_LISTENER_HPP
#define CONNECTION_LISTENER_HPP

#include <string>
#include <memory>

#include "mysocket.hpp"
#include <memory>


class Connection;

class ConnectionListener
{
public:

    // Only works with IPv6. ::1
    // @port: Port that this socket will listen to
    // @ai_socktype: SOCK_DGRAM for UDP, SOCK_STREAM for TCP
    ConnectionListener(int port, int ai_socktype);

    ~ConnectionListener();

    int GetPort();
    int GetSockType();

    // Listen for incoming connections
    // Block the program until a connection is received
    // Should be used in a separate thread. I don't know. You tell me!
    void Listen();

    // Return a Connection for sending & receiving messages
    // It is a shared object to prevent accidental socket closing
    // because of object destruction
    std::shared_ptr<Connection> Accept();

    // Stop listening for new connections
    void Close();


private:
    // getaddrinfo() results
    // addrinfo* _adList;

    addrinfo _ad;
    sockaddr_storage _ss; 

    // socket() result. 
    // Used to listen for incoming connections
    int _mySock;

    // There's no way to test if the socket is still open
    // other than to write to the socket and check error returns
    // @closed simplify the logic
    bool _closed;
};

#endif /* CONNECTION_LISTENER_HPP */