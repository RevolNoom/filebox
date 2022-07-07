#include "ActiveUser.hpp"

ActiveUser::ActiveUser(std::shared_ptr<Connection> connection):
    _connection(connection)
{
}

std::string ActiveUser::GetCommand()
{
    return _connection->Receive();
}

void ActiveUser::ReceiveAnswer(const std::string& ans)
{
    _connection->Send(ans);
}

std::shared_ptr<Connection> ActiveUser::GetConnection()
{
    return _connection;
}

Filesystem& ActiveUser::GetFileSystem()
{
    return _fs;
}