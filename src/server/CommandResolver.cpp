#include "CommandResolver.hpp"
#include <iostream>

CommandResolver::CommandResolver(): _serverFs("/"), _alive(true)
{
    _consumeCommands = std::thread([this](){this->ConsumeCommands();});
}

CommandResolver::~CommandResolver()
{
    _alive = false;
    _consumeCommands.join();
}

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
    // TODO: Does accessing deque while reallocating
    // cause segmentation fault?
    while (_alive)
    {
        if (_cmdQueue.begin() == _cmdQueue.end())
            continue;

        auto command = _cmdQueue.front();
        _cmdQueue.pop_front();

        if (command.second.substr(0, 3) == "up ")
            ReceiveFile(command);
        else if (command.second.substr(0, 3) == "ls ")
            SendFilesystemTree(command);
        else if (command.second.substr(0, 5) == "down ")
            SendFile(command);
        
    }
}

void CommandResolver::SendFile(CommandResolver::Command &C)
{
    std::string filename = C.second.substr(std::string("down ").length(), C.second.length() - std::string("down \n").length());
}

void CommandResolver::SendFilesystemTree(CommandResolver::Command &C)
{
    // TODO: Currently ignoring timestamp
    C.first.ReceiveAnswer("update_ls " + _serverFs.GetRecursiveFileList());
}

void CommandResolver::ReceiveFile(CommandResolver::Command &C)
{

}
