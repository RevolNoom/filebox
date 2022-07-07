#include "CommandResolver.hpp"
#include <iostream>
#include <fstream>

CommandResolver::CommandResolver(): _alive(true)
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
    using FUNCTION_NAME = std::string;
    using HANDLE_FUNCTION = void (CommandResolver::*)(CommandResolver::Command&);
    std::pair<FUNCTION_NAME, HANDLE_FUNCTION> cmd[] {
        {"cd", &CommandResolver::cd},
        {"ls", &CommandResolver::ls},
        {"up", &CommandResolver::ReceiveFile},
        {"down", &CommandResolver::SendFile},
        {"", &CommandResolver::commandNotFound}
    };

    // TODO: Does accessing deque while reallocating
    // cause segmentation fault?
    while (_alive)
    {
        if (_cmdQueue.begin() == _cmdQueue.end())
            continue;

        auto command = _cmdQueue.front();
        _cmdQueue.pop_front();
        std::cout<<"Command received: "<<command.second<<"\n";

        for (auto &c: cmd)
            if (command.second.substr(0, c.first.length()) == c.first)
            {
                (this->*c.second)(command);
                break;
            }
    }
}

void CommandResolver::SendFile(CommandResolver::Command &C)
{
    std::string filename = C.second.substr(std::string("down ").length(), C.second.length() - std::string("down \n").length());
}

void CommandResolver::ls(CommandResolver::Command &c)
{
    c.first.ReceiveAnswer("ok_ls " + c.first.GetFileSystem().ls(c.second.substr(3)));
}

void CommandResolver::ReceiveFile(CommandResolver::Command &C)
{
    // Dissect the packet
    std::string metadata(C.second.substr(C.second.find_last_of(' ')+1));
    metadata.pop_back(); // Remove trailing '\n'
    std::cout<<"Metadata: '"<<metadata<<"'\n";
    std::filesystem::path filename(metadata.substr(0, metadata.find_first_of(':')));
    std::cout<<"filename: '"<<filename.string()<<"'\n";
        
    //TODO: Currently, file size is ignored
    // int filesize = ???; Check for system memory availability

    std::filesystem::path destinationPath(C.second.substr(C.second.find_first_of(' ')+1));
    destinationPath.assign(destinationPath.string().substr(0, destinationPath.string().find_last_of(' ')));


    // See if there's any files with same name or not
    destinationPath /= filename.string();
    std::cout<<"Destination Path: "<<destinationPath.string()<<"\n";
    if (std::filesystem::exists(destinationPath))
    {
        C.first.ReceiveAnswer("no_up FILE_EXIST\n");
        return;
    }

    C.first.GetConnection()->Send("ok_up\n");
    std::fstream newFile(destinationPath.string());
    std::string content;
    while (1)
    {
        content = C.first.GetConnection()->Receive();
        if (content != "")
        {
            std::cout<<"Received content: '"<<content<<"'\n";
            newFile<<content;
            newFile.flush();
            break;
        }
    }
    std::cout<<"File received.\n";
}

void CommandResolver::cd(CommandResolver::Command &C)
{
    try
    {
        C.first.GetFileSystem().cd(C.second.substr(3));
    }
    catch (std::exception& e)
    {
        C.first.ReceiveAnswer("no_cd CANNOT_ACCESS\n");
        return;
    }
    C.first.ReceiveAnswer("ok_cd\n");
}

void CommandResolver::commandNotFound(CommandResolver::Command &C)
{
    // Just do nothin
}