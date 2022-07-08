#include <iostream>

#include "ServerFTP.hpp"
#include "Connection.hpp"
#include <fstream>

int main(int argc, char *argv[])
{
    std::cout<<"Booting server\n";
    ServerFTP s;
    s.Start();

    /*
    Filesystem f;
    std::cout<<f.ls(".gitignore")<<"\n";
    std::cout<<f.CanWrite("/")<<"\n";
    */
    return 0;
}