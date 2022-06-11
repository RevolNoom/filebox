#include <iostream>

#include "Connection.hpp"

int Main(int argc, char *argv[])
{
    //Filesystem f;
    //std::cout<<f.GetRecursiveFileList();
    //std::cout<<f.GetFile("a.out").GetContent();
    Connection t("::1", 8888, SOCK_STREAM);
    t.Send("Hello");
    std::cout<<"Server sends back: "<<t.Receive()<<"\n";

    return 0;
}