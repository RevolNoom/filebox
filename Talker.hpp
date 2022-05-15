#ifndef TALKER_HPP
#define TALKER_HPP

#include "mysocket.hpp"

// This socket is used for sending & receiving messages 
class Talker
{
public:
    // Try to connect to ip:port destination
    Talker(const std::string &ip, int port, int ai_socktype);
    ~Talker();

    void Send(const std::string& message);
    std::string Receive();

    std::string GetRemoteIP();
    int GetPort();
    int GetSockType();

private:
    friend class ConnectionListener;
    Talker(int acceptedSocket, sockaddr_storage sockInfo, int sockType);

    // I say it's an IPv6 if it has a ':' in it
    bool IsIPv6QuickCheck(const std::string &ip);
    addrinfo* GetAddrInfo(const std::string &ip, int port, int ai_socktype);

private:
    int _sock;
    int _sockType;
    sockaddr_storage _remoteInfo;

    char _recvBuffer[1000];
};

#endif /* TALKER_HPP */