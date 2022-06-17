#include <iostream>

#include "Connection.hpp"
#include "ServerFTP.hpp"

int Main(int argc, char *argv[])
{
    Connection c("::1", ServerFTP::PORT, SOCK_STREAM);
    c.Send("log abcd:123\n");
    std::cout<<"Server sends back: \""<<c.Receive()<<"\"\n";

    return 0;
}