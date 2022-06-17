#ifndef MYSOCKET_HPP
#define MYSOCKET_HPP

// Low-level libraries includes

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

    // For wtf, I have no ideas 
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


// Macro for readability
#ifndef ON_THREAD
#define ON_THREAD
#endif

#endif /* MYSOCKET_HPP */
