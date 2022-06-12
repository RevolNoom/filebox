#ifndef COMMAND_RESOLVER_HPP
#define COMMAND_RESOLVER_HPP

#include <deque>
#include "ActiveUser.hpp"

// Serve client commands
class CommandResolver
{
public:
    void Resolve(ActiveUser& user);

private:

    void ConsumeCommands() ON_THREAD;

    void SendFile();
    void SendFilesystemTree();
    void ReceiveFile();

private:
    // int _maxWorkerThread;

    using Command = std::pair<ActiveUser, std::string>;
    std::deque<Command> _cmdQueue;
};

#endif