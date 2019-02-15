//
// Created by Brian Lochan on 2019-02-11.
//

#include <sys/proc_info.h>
#include "Client.h"

using namespace std;

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
    cout << "creating socket with return value: " << sock << endl;

    // check if the socket is valid
    sock_valid(sock);

    // link an IP adress to the socket
   struct addrinfo hint;
   struct addrinfo *results;
   //int info;

   hint.ai_family = AF_INET; // AF_UNSPEC
   hint.ai_socktype = SOCK_STREAM; // SOCK_DGRAM
   hint.ai_protocol = 0;

    int test  = getaddrinfo("52.58.97.202", "5378", &hint, &results);

    //(connect(sock, (struct addrinfo *)results, sizeof(results)));

  //  addrinfo client;
//    int size = sizeof(client);


}

void Client::closeSocket() {

}

Client::Client(SOCKET sock) : sock(sock) {

}


