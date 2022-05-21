#include <iostream>

#include "mysocket.hpp"
#include "ConnectionListener.hpp"
#include "Talker.hpp"

int Main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    /* Quoted from Beej's guide:

        "You also have to tell your compiler to link in the Winsock library, usually called wsock32.lib or
        winsock32.lib, or ws2_32.lib for Winsock 2.0. Under VC++, this can be done through the Project
        menu, under Settings.... Click the Link tab, and look for the box titled “Object/library modules”.
        Add “wsock32.lib” (or whichever lib is your preference) to that list."
    */
    #ifdef _WIN32
        WSADATA wsaData;
        // if this doesn't work
        //WSAData wsaData; // then try this instead
        // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:
        if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0) 
        {
            fprintf(stderr, "WSAStartup failed.\n");
            exit(1);
        }

        mainReal(argc, argv);

        WSACleanup();
    #else
        Main(argc, argv);
    #endif

    return 0;
}
