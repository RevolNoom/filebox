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
        for (int iii = 0; iii < _pendingConnection.size(); ++iii)
        {
            auto credential = _pendingConnection[iii]->Receive(); 
            if (credential != "")
            {
                if (credential == std::string("log ") + _credential.first + ":" + _credential.second + "\n")
                {
                    _pendingConnection[iii]->Send("ok_log");
                    _employer->AcceptNewUser(ActiveUser(_pendingConnection[iii]));
                    _pendingConnection.erase(_pendingConnection.begin()+iii);
                    
                }
                else 
                    _pendingConnection[iii]->Send("no_log WRONG_CREDENTIAL");


            }
        }
    }    
}

std::pair<Authenticator::IP, Authenticator::PORT> Authenticator::GetListenerInfo()
{
    //TODO:
    return {"1", _listener.GetPort()};
}
