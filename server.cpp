#include <iostream>

#include "mysocket.hpp"
#include "ConnectionListener.hpp"
#include "Talker.hpp"

int Main(int argc, char *argv[])
{
    ConnectionListener c(8888, SOCK_STREAM);
    c.Listen();
    auto t = c.Accept();
    //std::cout<<"Accepted Talker: "<<t.GetRemoteIP()<<":"<<t.GetPort()<<"\n";
    std::cout<<"Client says: "<<t.Receive()<<"\n";
    t.Send("Hey");

    return 0;
}