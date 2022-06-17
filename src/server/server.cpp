#include <iostream>

#include "ServerFTP.hpp"
#include "Connection.hpp"

int Main(int argc, char *argv[])
{
    std::cout<<"Booting server\n";
    ServerFTP s;

    s.Start();

    //std::cout<<"Connecting\n";
    /*
    Connection c("::1", ServerFTP::PORT, SOCK_STREAM);
    c.Send("log abcd:123\n");
    std::cout<<"Server sends back: "<<c.Receive()<<"\n";
    */
   
    return 0;
}