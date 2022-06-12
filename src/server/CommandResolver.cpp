#include "CommandResolver.hpp"

void CommandResolver::Resolve(ActiveUser& user)
{
    while (user.HasCommand())
    {
        _cmdQueue.push_back({user, user.GetCommand()});
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
