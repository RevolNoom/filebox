#ifndef MUTEX_CONNECTION_HPP 
#define MUTEX_CONNECTION_HPP

#include "Connection.hpp"
#include <mutex>

// DECORATOR for Connection
class MutexConnection
{
public:
    MutexConnection();
    ~MutexConnection();

    void ChangeConnection(std::shared_ptr<Connection> c);
    void Terminate();

    void Send(const std::string& message);

    std::string Receive();

    std::string GetRemoteIP() const;

private:
    std::shared_ptr<Connection> _c;
    std::mutex m;
};

#endif