#ifndef ACTIVE_USER_HPP
#define ACTIVE_USER_HPP

#include <deque>
#include <memory>
#include <string>
#include "mysocket.hpp"
#include "Filesystem.hpp"
#include "Connection.hpp"

// TODO: This is a wrapper for Connection
// Is it necessary???
class ActiveUser
{
public:
    ActiveUser(std::shared_ptr<Connection> connection);

    // Consume one command from user's list.
    std::string GetCommand();

    // This is not good. I'm not supposed to expose
    // _connection like this
    std::shared_ptr<Connection> GetConnection();

    Filesystem& GetFileSystem();
    void ReceiveAnswer(const std::string& ans);
    
private:
    std::shared_ptr<Connection> _connection;
    Filesystem _fs;
};

#endif