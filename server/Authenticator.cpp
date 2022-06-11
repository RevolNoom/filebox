#include "Authenticator.hpp"
#include "Connection.hpp"
#include "ServerFTP.hpp"

Authenticator::Authenticator(ServerFTP* employer): 
    _employer(employer), 
    _listener(9999, SOCK_STREAM),
    _alive(true)
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
            if (credential == std::string("log ") + _credential.first + ":" + _credential.second + "\n")
            {
                _employer->AcceptNewUser(ActiveUser(_pendingConnection[iii]));
            }
        }
    }    
}

std::pair<Authenticator::IP, Authenticator::PORT> Authenticator::GetListenerInfo()
{
    //TODO:
    return {"1", 2};
}
