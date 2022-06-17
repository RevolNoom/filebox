#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "mysocket.hpp"
#include <deque>
#include <sstream>

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

    // TODO: Convert size header according to endian-ness 
    void Send(const std::string& message);

    std::string Receive();

    std::string GetRemoteIP() const;

    //int GetPort();
    //int GetSockType();

    // Don't use it. I haven't decided on 
    // how to implement state yet
    //STATE GetState();

private:

    union Int64ToRawBytes
    {
        int64_t _int;
        char _str[8];
    };
    static constexpr int MESSAGE_HEADER_SIZE = sizeof(Int64ToRawBytes);

    friend class ConnectionListener;
    Connection(int acceptedSocket, sockaddr_storage sockInfo, int sockType);

    // I say it's an IPv6 if it has a ':' in it
    bool IsIPv6QuickCheck(const std::string &ip) const;
    addrinfo* GetAddrInfo(const std::string &ip, int port, int ai_socktype) const;

    class Buffer
    {
    public:

        void Write(const char* src, int count);
        // Return the message minus the message header
        // or "" if there's no message available 
        std::string GetMessage();
    
    private:
        void WriteFront(const char* src, int count);

        // No, GetFront() is not public.
        // Use GetMessage(). 
        // Don't try to do anything funny with my buffer
        std::string GetFront(int count);

    private:
        std::deque<char> _recvBuffer;
    };

private:

    int _sock;
    int _sockType;
    sockaddr_storage _remoteInfo;

    Buffer _recvBuffer;

    //STATE _state;
    static constexpr size_t MAX_BUFFER_SIZE = 1000;
};

#endif /* CONNECTION_HPP */