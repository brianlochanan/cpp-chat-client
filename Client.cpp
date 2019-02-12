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
    sock_init();
}

void Client::closeSocket() {

}

Client::Client(SOCKET sock) : sock(sock) {

}


