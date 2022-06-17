/*#include "BackgroundConnectionUpdater.hpp"
#include <iostream>

BackgroundConnectionUpdater BackgroundConnectionUpdater::_singleton = BackgroundConnectionUpdater();

BackgroundConnectionUpdater& BackgroundConnectionUpdater::Singleton()
{
    return _singleton;
}

BackgroundConnectionUpdater::BackgroundConnectionUpdater():
    _alive(true),
    _MAXnfds(0)
{
    _pollUpdating = std::thread([this](){PollUpdating();});
}

BackgroundConnectionUpdater::~BackgroundConnectionUpdater()
{
    _alive = false;
    _pollUpdating.join();
}

void BackgroundConnectionUpdater::Subscribe(Connection* c)
{
    FD_SET(c->GetFileDescriptor(), &_masterFDSet);
    _subscribedConnections.push_back({c->GetFileDescriptor(), c});
    _MAXnfds = std::max(_MAXnfds, c->GetFileDescriptor());
}

void BackgroundConnectionUpdater::Unsubscribe(Connection* c)
{
    FD_CLR(c->GetFileDescriptor(), &_masterFDSet);

    int secondHighestFD=0;

    for (int iii = 0; iii<_subscribedConnections.size(); ++iii)
    {
        if (_subscribedConnections[iii].first == c->GetFileDescriptor())
        {
            _subscribedConnections.erase(_subscribedConnections.begin()+iii);
        }

        if (secondHighestFD > _subscribedConnections[iii].first &&
            secondHighestFD < _MAXnfds)
            secondHighestFD = _subscribedConnections[iii].first;
    }

    _MAXnfds = secondHighestFD;
}

void BackgroundConnectionUpdater::PollUpdating()
{
    while (_alive)
    {
        fd_set temp = _masterFDSet;
        select(_MAXnfds + 1, &temp, NULL, NULL, NULL);

        for (auto &connection: _subscribedConnections)
        {
            if (FD_ISSET(connection.first, &temp));
            {
                std::cout<<"Selected: "<<connection.first<<"\n";
                connection.second->FetchBuffer();
            }
        }
    }
}*/