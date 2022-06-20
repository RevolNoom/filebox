#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include <string>
#include <thread>
#include <list>
#include "ConnectionListener.hpp"

class ServerFTP;

// Authenticate new connections
class Authenticator
{
public:

    // Spawns 2 threads to listen new users and authenticate
    // Non-blocking
    void Start();
    ~Authenticator();

    using USERNAME=std::string;
    using PASSWORD=std::string;
    using Credential=std::pair<USERNAME, PASSWORD>;
    const Credential& GetCredential();

    using IP=std::string;
    using PORT=int;
    std::pair<IP, PORT> GetListenerInfo();

    Authenticator(ServerFTP* employer);

private:

    friend class ServerFTP;
    Credential GenerateCredential();

    void ListenNewUsers() ON_THREAD;
    
    void Authenticate() ON_THREAD;

private:
    ConnectionListener _listener;

    std::list<std::shared_ptr<Connection>> _pendingConnection;

    // The one who inited this Authenticator
    ServerFTP* _employer;

    Credential _credential;

    std::vector<std::thread> _threads;
    bool _alive;
};

#endif