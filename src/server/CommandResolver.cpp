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
    auto filename = C.second.substr(5);
    filename.pop_back(); // Remove trailing \n

    auto downFile = C.first.GetFileSystem().GetFile(filename);
    if (!downFile.CanRead())
    {
        C.first.ReceiveAnswer("no_down CANT_READ_FILE");
        return;
    }

    C.first.ReceiveAnswer("ok_down " + std::to_string(downFile.GetSize()) + "\n");

    auto confirmation = C.first.GetConnection()->Receive();
    if (confirmation == "ok_down\n")
        C.first.ReceiveAnswer(downFile.GetContent());
}

void CommandResolver::ls(CommandResolver::Command &c)
{
    c.first.ReceiveAnswer("ok_ls " + c.first.GetFileSystem().ls(c.second.substr(3)));
}

void CommandResolver::ReceiveFile(CommandResolver::Command &C)
{
    // Dissect the packet
    int beginFilepath = C.second.find_first_of(' '),
        beginFilesize = C.second.find_last_of(' ');

    std::filesystem::path filepath(C.second.substr(beginFilepath+1, beginFilesize-beginFilepath-1));
    std::cout<<"filepath: '"<<filepath.string()<<"'\n";
    //TODO: Currently, file size is ignored
    // int filesize = ???; Check for system memory availability

    auto file = C.first.GetFileSystem().GetFile(filepath);
    if (!file.CanWrite())
        C.first.ReceiveAnswer("no_up PERMISSION_DENIED\n");


    if (file.Exist())
    {
        C.first.ReceiveAnswer("no_up FILE_EXIST\n");
        return;
    }

    C.first.GetConnection()->Send("ok_up\n");
    std::string content;
    while (1)
    {
        content = C.first.GetConnection()->Receive();
        if (content != "")
        {
            std::cout<<"Received content: '"<<content<<"'\n";
            file.WriteContent(content);
            break;
        }
    }
    std::cout<<"File received.\n";
}

void CommandResolver::cd(CommandResolver::Command &C)
{
    std::string lsResult;
    try
    {
        lsResult = C.first.GetFileSystem().ls(C.second.substr(3));
    }
    catch(const std::exception& e)
    {
        C.first.ReceiveAnswer("no_cd PERMISSION_DENIED\n");
        std::cout<<"cd Exception: "<<e.what()<<"\n";
        return;
    }
    
    if (lsResult.back() == '\n')
        C.first.ReceiveAnswer("ok_cd\n");
    else
        C.first.ReceiveAnswer("no_cd NOT_DIRECTORY\n");
}

void CommandResolver::commandNotFound(CommandResolver::Command &C)
{
    // Just do nothin
}