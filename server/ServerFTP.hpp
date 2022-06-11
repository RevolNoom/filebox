#ifndef SERVER_FTP
#define SERVER_FTP

#ifndef ON_THREAD
#define ON_THREAD
#endif

#include <thread>
#include <deque>

#include "mysocket.hpp"
#include "ConnectionListener.hpp"
#include "Authenticator.hpp"
#include "CommandResolver.hpp"

class ServerFTP
{
public:
    ServerFTP();
    ~ServerFTP();

    void Start();

private:
    friend class Authenticator;
    void AcceptNewUser(const ActiveUser& user);

private: 

    void UpdateUserCommands() ON_THREAD;

    void ResolveCommands() ON_THREAD;

private:
    CommandResolver _cmdr;
    Authenticator _auth;

    // TODO: There has to be some ways to supervise
    // dropped connections.
    std::vector<ActiveUser> _users;

};



#endif /* SERVER_FTP */