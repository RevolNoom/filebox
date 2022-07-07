#include <iostream>

#include "ServerFTP.hpp"
#include "Connection.hpp"

int main(int argc, char *argv[])
{
    std::cout<<"Booting server\n";
    ServerFTP s;
    s.Start();
    /*
    Filesystem f;
    std::cout<<f.ls("/Misc")<<"\n";
    */

    //std::cout<<"Connecting\n";
    /*
    Connection c("::1", ServerFTP::PORT, SOCK_STREAM);
    c.Send("log abcd:123\n");
    std::cout<<"Server sends back: "<<c.Receive()<<"\n";
    */
   
    return 0;
}