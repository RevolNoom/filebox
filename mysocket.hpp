#ifndef MYSOCKET_HPP
#define MYSOCKET_HPP

#ifdef _WIN32

    #include <winsock.h>

#else

    // For close()
    #include <unistd.h>

    // For send(), recv(), bla bla...
    #include <sys/socket.h>

    // For addrinfo and tons of other structs
    #include <netdb.h>

    // For inet_ntop
    #include <arpa/inet.h>

    // For memset
    #include <string.h>

    // For all network calls.
    // They all set errno 
    #include <errno.h>

    #include <string>

#endif /*_WIN32*/

#endif /* MYSOCKET_HPP */
