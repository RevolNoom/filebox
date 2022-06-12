#ifndef ACTIVE_USER_HPP
#define ACTIVE_USER_HPP

#include <deque>
#include <memory>
#include <string>

#ifndef ON_THREAD
#define ON_THREAD
#endif

class Connection;

class ActiveUser
{
public:
    ActiveUser(std::shared_ptr<Connection> connection);

    // Update the list of this user's command 
    void UpdateCommand() ON_THREAD;

    // True if GetCommand() will return something
    bool HasCommand() const;

    // Consume one command from user's list.
    const std::string& GetCommand();

    // void ReceiveAnswer(const std::string& ans);
    
private:
    std::deque<std::string> _commandQueue;
    std::shared_ptr<Connection> _connection;
};

#endif