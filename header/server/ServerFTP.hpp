#ifndef SERVER_FTP
#define SERVER_FTP

#include <thread>
#include <list>

#include "mysocket.hpp"
#include "ConnectionListener.hpp"
#include "Authenticator.hpp"
#include "CommandResolver.hpp"

class ServerFTP
{
public:

    static constexpr int PORT = 9999;

    ServerFTP();
    ~ServerFTP();

    void Start();

private:
    friend class Authenticator;
    void AcceptNewUser(const ActiveUser& user);

private: 

    //void ResolveCommands() ON_THREAD;

private:
    CommandResolver _cmdr;
    Authenticator _auth;

    // TODO: There has to be some ways to supervise
    // dropped connections.
    std::list<ActiveUser> _users;

};



#endif /* SERVER_FTP */