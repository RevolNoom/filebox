#ifndef COMMAND_RESOLVER_HPP
#define COMMAND_RESOLVER_HPP

#include <list>
#include <thread>
#include "ActiveUser.hpp"
#include "Filesystem.hpp"

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

    // Send file to user when they request download
    void SendFile(CommandResolver::Command &C);

    // Send filesystem when they request ls
    void SendFilesystemTree(CommandResolver::Command &C);

    // Receive file from user when they request upload
    void ReceiveFile(CommandResolver::Command &C);

private:
    // int _maxWorkerThread;

    bool _alive;
    std::thread _consumeCommands;

    std::list<Command> _cmdQueue;
    Filesystem _serverFs;
};

#endif