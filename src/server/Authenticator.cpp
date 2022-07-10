#include "Authenticator.hpp"
#include "Connection.hpp"
#include "ServerFTP.hpp"
#include <iostream>

Authenticator::Authenticator(ServerFTP* employer): 
    _employer(employer), 
    _listener(ServerFTP::PORT, SOCK_STREAM),
    _alive(true),
    _pendingConnection()
{
    // Accessing a Connection
    // while std::vector is re-allocating causes segmentation fault
    // std::list does not break the system,
    // but we're chasing pointers across the system.
    //_pendingConnection.reserve(100);

    _credential = GenerateCredential();
}

Authenticator::~Authenticator()
{
    _alive = false;
    for (auto &t: _threads)
        t.join();
}

Authenticator::Credential Authenticator::GenerateCredential()
{
    return {"a", "a"};
}

const Authenticator::Credential& Authenticator::GetCredential()
{
    return _credential;
}

void Authenticator::Start()
{
    _threads.push_back(std::thread([this](){this->ListenNewUsers();}));
    _threads.push_back(std::thread([this](){this->Authenticate();}));
}


void Authenticator::ListenNewUsers()
{
    while (_alive)
    {
        _listener.Listen();
        _pendingConnection.push_back(_listener.Accept());
        std::cout<<"Authenticator got a new user!\n";
    }
}

void Authenticator::Authenticate()
{
    while (_alive)
    {
        if (_pendingConnection.size() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
            
        for (auto it_connection = _pendingConnection.begin();
            it_connection != _pendingConnection.end();)
        {
            auto connection = *it_connection;
            if (connection->IsClosed())
            {
                auto temp = it_connection++;
                _pendingConnection.erase(temp);
                continue;
            }

            auto credential = connection->Receive(); 
            if (credential != "")
            {
                std::cout<<"Received credential: "<<credential<<"\n";
                if (credential == std::string("log ") + _credential.first + ":" + _credential.second + "\n")
                {
                    connection->Send("ok_log\n");
                    std::cout<<"Sent ok_log\n";
                    _employer->AcceptNewUser(ActiveUser(connection));
                    auto temp = it_connection++;
                    _pendingConnection.erase(temp);
                    continue;
                }
                else 
                {
                    connection->Send("no_log WRONG_CREDENTIAL\n");
                    std::cout<<"Sent no_log\n";
                }
            }

            // Iterator increment is put here
            // to skip cases when I need to erase connections.
            ++it_connection;
        }
    }    
}

std::pair<Authenticator::IP, Authenticator::PORT> Authenticator::GetListenerInfo()
{
    //TODO:
    return {"1", _listener.GetPort()};
}
