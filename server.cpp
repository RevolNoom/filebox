#include <iostream>

#include "mysocket.hpp"
#include "ConnectionListener.hpp"
#include "Talker.hpp"

int main()
{

    ConnectionListener c(8888, SOCK_STREAM);
    c.Listen();
    auto t = c.Accept();
    std::cout<<"Client says: "<<t.Receive()<<"\n";
    t.Send("Hey");

    return 0;
}