#ifndef TALKER_HPP
#define TALKER_HPP

#include <string>

// For send(), recv(), bla bla...
#include <sys/socket.h>

// For addrinfo and tons of other structs
#include <netdb.h>

// For inet_ntop
#include <arpa/inet.h>

// This socket is used for sending & receiving messages 
class Talker
{
public:
    // Try to connect to ip:port destination
    Talker(const std::string &ip, int port, int ai_socktype);

    //TODO: 
    virtual void Send(const std::string& message);// = 0;
    virtual std::string Receive();// = 0;

    std::string GetRemoteIP();
    int GetSockType();

private:
    friend class ConnectionListener;
    Talker(int acceptedSocket, sockaddr_storage sockInfo);

private:
    int _sock;
    sockaddr_storage _remoteInfo;

    char _recvBuffer[1000];
};

#endif /* TALKER_HPP */