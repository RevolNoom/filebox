#include <iostream>

#include "ConnectionListener.hpp"
#include "Talker.hpp"

int Main(int argc, char *argv[])
{

    //ConnectionListener c(9999, SOCK_STREAM);
    //c.Listen();
    //c.Accept();

    Talker t("127.0.0.1", 8888, SOCK_STREAM);
    t.Send("Hello");
    std::cout<<"Server sends back: "<<t.Receive()<<"\n";

    return 0;
}