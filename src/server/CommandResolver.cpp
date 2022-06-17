#include "CommandResolver.hpp"

void CommandResolver::Resolve(ActiveUser& user)
{
    std::string command;
    while ((command = user.GetCommand()) != "")
    {
        _cmdQueue.push_back({user, command});
    }
}

void CommandResolver::ConsumeCommands()
{
    while (true)
    {
        auto command = _cmdQueue.front();
        _cmdQueue.pop_front();

        // TODO: Multiplexing commands

        /*
        up
        down
        ls
        */
    }
}

void CommandResolver::SendFile()
{

}

void CommandResolver::SendFilesystemTree()
{

}

void CommandResolver::ReceiveFile()
{

}
