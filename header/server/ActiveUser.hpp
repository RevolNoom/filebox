#ifndef ACTIVE_USER_HPP
#define ACTIVE_USER_HPP

#include <deque>
#include <memory>
#include <string>
#include "mysocket.hpp"

class Connection;

// TODO: This is a wrapper for Connection
// Is it necessary???
class ActiveUser
{
public:
    ActiveUser(std::shared_ptr<Connection> connection);

    // Consume one command from user's list.
    std::string GetCommand();

    void ReceiveAnswer(const std::string& ans);
    
private:
    std::shared_ptr<Connection> _connection;
};

#endif