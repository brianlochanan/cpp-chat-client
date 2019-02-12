//
// Created by Brian Lochan on 2019-02-11.
//

#include <sys/proc_info.h>
#include "Client.h"

void Client::tick() {

}

int Client::readFromStdin() {
    return 0;
}

int Client::readFromSocket() {
    return 0;
}

void Client::createSocketAndLogIn() {
    // initialize the socket
    sock_init();

    // create the socket (domain(ipv4), type(tcp), protocol(0/IP))
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    // check if the socket is valid
    sock_valid(sock);

    // link an IP adress to the socket



}

void Client::closeSocket() {

}

Client::Client(SOCKET sock) : sock(sock) {

}


