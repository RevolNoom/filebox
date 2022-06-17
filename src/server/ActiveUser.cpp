#include "ActiveUser.hpp"
#include "Connection.hpp"

ActiveUser::ActiveUser(std::shared_ptr<Connection> connection):
    _connection(connection)
{
}

std::string ActiveUser::GetCommand()
{
    return _connection->Receive();
}

// void ReceiveAnswer(const std::string& ans);
