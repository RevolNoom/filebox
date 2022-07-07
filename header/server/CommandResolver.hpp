#ifndef COMMAND_RESOLVER_HPP
#define COMMAND_RESOLVER_HPP

#include <list>
#include <thread>
#include "Filesystem.hpp"
#include "ActiveUser.hpp"

// Serve client commands
class CommandResolver
{
public:
    CommandResolver();
    ~CommandResolver();

    void Resolve(ActiveUser& user);

private:
    using Command = std::pair<ActiveUser, std::string>;

    void ConsumeCommands() ON_THREAD;

    // Change user's working directory 
    void cd(CommandResolver::Command &C);

    // Send file to user when they request download
    void SendFile(CommandResolver::Command &C);

    // Send filesystem when they request ls
    void ls(CommandResolver::Command &C);

    // Receive file from user when they request upload
    void ReceiveFile(CommandResolver::Command &C);

    void commandNotFound(CommandResolver::Command &C);

private:
    // int _maxWorkerThread;

    bool _alive;
    std::thread _consumeCommands;

    std::list<Command> _cmdQueue;
};

#endif