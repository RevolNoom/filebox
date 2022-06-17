#ifndef ACTIVE_USER_HPP
#define ACTIVE_USER_HPP

#include <deque>
#include <memory>
#include <string>

#ifndef ON_THREAD
#define ON_THREAD
#endif

class Connection;

// Intended to bind Username and a connection...
// We'll see about that
class ActiveUser
{
public:
    ActiveUser(std::shared_ptr<Connection> connection);

    // Consume one command from user's list.
    std::string GetCommand();

    // void ReceiveAnswer(const std::string& ans);
    
private:
    std::shared_ptr<Connection> _connection;
};

#endif