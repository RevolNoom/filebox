#include <iostream>

#include "mysocket.hpp"
#include "ConnectionListener.hpp"
#include "Connection.hpp"

int Main(int argc, char *argv[])
{
    //Filesystem f;
    //std::cout<<f.GetRecursiveFileList();
    //std::cout<<f.GetFile("client.cpp").GetContent();
    ConnectionListener c(8888, SOCK_STREAM);
    c.Listen();
    auto t = c.Accept();
    std::cout<<"Client says: "<<t.Receive()<<"\n";
    t.Send("Hey");

    return 0;
}