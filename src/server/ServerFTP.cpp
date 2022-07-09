#include "ServerFTP.hpp"
#include "Connection.hpp"
#include <iostream>


ServerFTP::ServerFTP(): _auth(this)
{

}


ServerFTP::~ServerFTP()
{

}

void ServerFTP::AcceptNewUser(const ActiveUser& user)
{
    _users.push_back(user);
    std::cout<<"Got a new user!\n";
}

void ServerFTP::Start()
{
    _auth.Start();
    while (true)
        for (auto it = _users.begin(); it != _users.end(); )
        {
            if (it->GetConnection()->IsClosed())
            {
                auto temp = it++;
                _users.erase(temp);
                continue;
            }
                
            _cmdr.Resolve(*(it++));
        }
}