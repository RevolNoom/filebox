#include "ActiveUser.hpp"
#include "Connection.hpp"

ActiveUser::ActiveUser(std::shared_ptr<Connection> connection):
    _connection(connection)
{
}

void ActiveUser::UpdateCommand()
{
    std::string command;
    while ((command = _connection->Receive()) != "")
        _commandQueue.push_back(command);
}

bool ActiveUser::HasCommand() const
{
    return _commandQueue.size();
}

const std::string& ActiveUser::GetCommand()
{
    const std::string& result = _commandQueue.front();
    _commandQueue.pop_front();
    return result;
}

// void ReceiveAnswer(const std::string& ans);
