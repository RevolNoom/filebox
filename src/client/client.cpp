#include <iostream>

#include "Connection.hpp"
#include "ServerFTP.hpp"

int Main(int argc, char *argv[])
{
    Connection c("::1", ServerFTP::PORT, SOCK_STREAM);
    c.Send("log abcd:1234\n");

    return 0;
}