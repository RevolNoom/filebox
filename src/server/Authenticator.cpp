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
    //std::cout<<"Authenticator joined all threads!";
}

Authenticator::Credential Authenticator::GenerateCredential()
{
    return {"abcd", "1234"};
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
        //std::cout<<"Listening\n";
        _listener.Listen();
        _pendingConnection.push_back(_listener.Accept());
    }
}

void Authenticator::Authenticate()
{
    while (_alive)
    {
        for (auto it_connection = _pendingConnection.begin();
            it_connection != _pendingConnection.end();
            ++it_connection)
        {
            auto connection = *it_connection;
            auto credential = connection->Receive(); 
            if (credential != "")
            {
                if (credential == std::string("log ") + _credential.first + ":" + _credential.second + "\n")
                {
                    connection->Send("ok_log\n");
                    _employer->AcceptNewUser(ActiveUser(connection));
                    _pendingConnection.erase(it_connection);
                }
                else 
                    connection->Send("no_log WRONG_CREDENTIAL\n");
            }
        }
    }    
}

std::pair<Authenticator::IP, Authenticator::PORT> Authenticator::GetListenerInfo()
{
    //TODO:
    return {"1", _listener.GetPort()};
}
