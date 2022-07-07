#include <iostream>

//#include "ServerFTP.hpp"
#include "Application.hpp"

int main(int argc, char *argv[])
{
    Application::singleton()->Execute();
    /*
    Connection c("::1", ServerFTP::PORT, SOCK_STREAM);
    c.Send("log abcd:1234\n");
    std::cout<<"Server sends back: \""<<c.Receive()<<"\"\n";
    c.Send("ls 123\n");
    std::cout<<"Server sends back: \""<<c.Receive()<<"\"\n";
    */

    return 0;
}