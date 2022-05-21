#include <iostream>

#include "ConnectionListener.hpp"
#include "Connection.hpp"

int Main(int argc, char *argv[])
{
    Connection t("127.0.0.1", 8888, SOCK_STREAM);
    t.Send("Hello");
    std::cout<<"Server sends back: "<<t.Receive()<<"\n";

    return 0;
}