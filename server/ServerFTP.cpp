#include "ServerFTP.hpp"
#include "Connection.hpp"



ServerFTP::ServerFTP(): _auth(this)
{

}


ServerFTP::~ServerFTP()
{

}


void ServerFTP::AcceptNewUser(const ActiveUser& user)
{
    _users.push_back(user);
}

void ServerFTP::Start()
{
    _auth.Start();
}