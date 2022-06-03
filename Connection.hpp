#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "mysocket.hpp"

void ThrowErrnoMsg(const std::string &msg);

// This socket is used for sending & receiving messages 
class Connection
{
public:

    /*
    enum STATE
    {
        CONNECTED,
        CLOSED,
        ERROR
    };
    */

    // Try to connect to ip:port destination
    Connection(const std::string &ip, int port, int ai_socktype);
    ~Connection();

    // TODO: loops send()ing until the whole message is transferred
    void Send(const std::string& message);

    std::string Receive();

    std::string GetRemoteIP();

    //int GetPort();
    //int GetSockType();

    // Don't use it. I haven't decided on 
    // how to implement state yet
    //STATE GetState();

private:
    friend class ConnectionListener;
    Connection(int acceptedSocket, sockaddr_storage sockInfo, int sockType);

    // I say it's an IPv6 if it has a ':' in it
    bool IsIPv6QuickCheck(const std::string &ip);
    addrinfo* GetAddrInfo(const std::string &ip, int port, int ai_socktype);

private:

    int _sock;
    int _sockType;
    sockaddr_storage _remoteInfo;

    //STATE _state;

    static constexpr size_t MAX_BUFFER_SIZE = 1000;
};

#endif /* CONNECTION_HPP */