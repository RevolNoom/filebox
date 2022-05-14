#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <memory>

// For send(), recv(), bla bla...
#include <sys/socket.h>

// For addrinfo and tons of other structs
#include <netdb.h>

// For inet_ntop
#include <arpa/inet.h>

// C-Socket wrapper

class Socket
{
public:

    virtual ~Socket();
};

#endif /* SOCKET_HPP */