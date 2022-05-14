#include "Socket.hpp"
#include <iostream>

#include "ConnectionListener.hpp"

#include "Talker.hpp"

int main()
{

    ConnectionListener c(8888, SOCK_STREAM);
    c.Listen();
    c.Accept();

    return 0;
}