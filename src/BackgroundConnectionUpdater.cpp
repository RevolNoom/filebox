#include "BackgroundConnectionUpdater.hpp"
#include <iostream>

BackgroundConnectionUpdater BackgroundConnectionUpdater::_singleton = BackgroundConnectionUpdater();

BackgroundConnectionUpdater& BackgroundConnectionUpdater::Singleton()
{
    return _singleton;
}

BackgroundConnectionUpdater::BackgroundConnectionUpdater():
    _alive(true),
    _subscriberProtector(),
    _masterFDSet()
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
    _subscriberProtector.lock();
    _subscribedConnections[c->GetFileDescriptor()] = c;
    _subscriberProtector.unlock();
    //std::cout<<"Subscribed: "<<c->GetFileDescriptor()<<"\n";
}

void BackgroundConnectionUpdater::Unsubscribe(Connection* c)
{
    FD_CLR(c->GetFileDescriptor(), &_masterFDSet);
    _subscriberProtector.lock();
    _subscribedConnections.erase(c->GetFileDescriptor());
    _subscriberProtector.unlock();
    //std::cout<<"Unsubscribed: "<<c->GetFileDescriptor()<<"\n";
}

void BackgroundConnectionUpdater::PollUpdating()
{
    timeval t;
    t.tv_sec = 0;
    t.tv_usec = 10000;
    while (_alive)
    {
        if (_subscribedConnections.size() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        
        fd_set temp = _masterFDSet;

        _subscriberProtector.lock();
        int MaxFD = (_subscribedConnections.rbegin())->first;
        //std::cout<< "MaxFD: "<<MaxFD<<"\n";
        _subscriberProtector.unlock();
        
        // F. Alright. I have to Set Damn timeval to 0
        // NULL will make select() block
        // Read man page better next time =.=
        auto selectStatus = select(MaxFD + 1, &temp, NULL, NULL, &t);
        /*
        if (selectStatus == -1)
        {
            std::cout<<"Select error!\n";
            continue;
        }
        else if (selectStatus == 0)
        {
            continue;
        }
        */

        _subscriberProtector.lock();
        for (auto &connection: _subscribedConnections)
        {
            //std::cout<<"> Selecting "<<connection.first<<"\n";
            if (FD_ISSET(connection.first, &temp));
            {
                //std::cout<<"> Selected "<<connection.first<<"\n";
                connection.second->FetchBuffer();
            }
        }
        _subscriberProtector.unlock();
    }
}