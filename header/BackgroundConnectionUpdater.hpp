/*#ifndef BACKGROUND_CONNECTION_UPDATER_HPP
#define BACKGROUND_CONNECTION_UPDATER_HPP

#include "mysocket.hpp"
#include <thread>
#include "Connection.hpp"
#include <vector>

// Spawn a thread to fetch Connection's buffer when it's available 
class BackgroundConnectionUpdater 
{
public:
    static BackgroundConnectionUpdater& Singleton();
    ~BackgroundConnectionUpdater();
    void Subscribe(Connection *c);
    void Unsubscribe(Connection *c);

private:
    static constexpr int MAX_FD_EVER = 20000;

    void PollUpdating() ON_THREAD;

    BackgroundConnectionUpdater();
    static BackgroundConnectionUpdater _singleton;

private:
    // Signal for PollUpdating() to stop
    bool _alive;
    std::thread _pollUpdating;

    fd_set _masterFDSet;
    std::vector<std::pair<int, Connection*>> _subscribedConnections;

    // Keep track of the largest-numbered file descriptor
    int _MAXnfds;
};

#endif /* BACKGROUND_CONNECTION_UPDATER_HPP */