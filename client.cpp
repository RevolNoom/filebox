#include "Socket.hpp"
#include <iostream>

#include "ConnectionListener.hpp"

#include "Talker.hpp"

int main()
{

    //ConnectionListener c(9999, SOCK_STREAM);
    //c.Listen();
    //c.Accept();

    Talker t("::1", 8888, SOCK_STREAM);

    return 0;
}