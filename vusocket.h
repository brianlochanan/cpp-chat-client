//
// Created by Jesse on 2019-01-03.
//

#ifndef CPP_CHAT_CLIENT_VUSOCKET_H
#define CPP_CHAT_CLIENT_VUSOCKET_H

/**
 * Socket example based on https://stackoverflow.com/questions/28027937/cross-platform-sockets
 */

#include <iostream>

//#define _WIN32

#ifdef _WIN32
    /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00   /* Windows 10. */
    #endif
    #include <winsock2.h>
    #include <Ws2tcpip.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
    #include <unistd.h> /* Needed for close() */
    #include <errno.h>

    #define BUF_SIZE 500

    void getaddr(){
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int sfd, s, j;
        size_t len;
        ssize_t nread;
        char buf[BUF_SIZE];

        /* Obtain address(es) matching host/port */

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
        hints.ai_flags = 0;
        hints.ai_protocol = 0;          /* Any protocol */

        s = getaddrinfo(argv[1], argv[2], &hints, &result);
        if (s != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            exit(EXIT_FAILURE);
        }
    }


    typedef int SOCKET;
    #define INVALID_SOCKET (SOCKET (~0))
    #define SOCKET_ERROR (-1)
#endif

/**
 * Must be called before creating and connecting sockets.
 *
 * @return zero on success.
 */
int sock_init();

/**
 * Checks if the given socket handle could be a valid socket handle. Even if this function returns true,
 * the handle might still point not point to a socket, or point to a disconnected socket.
 *
 * @param socket The socket to check.
 * @return false if the handle value is invalid.
 */
bool sock_valid(SOCKET socket);

/**
 * Retrieve the error code for the networking error that occured most recently.
 * @return the last error code.
 */
int sock_error_code();

/**
 * Close a connected socket.
 * @param sock The socket to close.
 * @return zero on success.
 */
int sock_close(SOCKET sock);

/**
 * Clean up resources allocated for working with sockets.
 * Call before program exit but after all sockets are closed.
 * @return zero on success.
 */
int sock_quit();

#endif //CPP_CHAT_CLIENT_VUSOCKET_H
