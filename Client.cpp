//
// Created by Brian Lochan on 2019-02-11.
//

#include <sys/proc_info.h>
#include "Client.h"

#define PORT 5378

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
    sock_init();

    int status;
    struct addrinfo hints, *res;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM; // SOCK_DGRAM
    hints.ai_flags = 0; //? voor de send function

    char *message;
    while (true){

        //get address of server
        status = getaddrinfo("52.58.97.202", "5378", &hints, &res);

        // create socket
        int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (!sock_valid(sockfd)) {
            cout << "Creating socket failed" << endl;
        };

        // connect socket with server
        int conn = connect(sockfd, res->ai_addr, res->ai_addrlen);

        // after connection free up memory
        freeaddrinfo(res);

        // input name
        string str;
        cout << "Input your name: " << endl;
        cin >> str;

        if (str == "!quit"){
            exit(1);
        }

        else{
            // send message to server

            string result;
            if(str == "!who"){
                result = "WHO\n";
            }
            else{
                message = "HELLO-FROM ";
                result = message + str + "\n";
            }

            const char *message = result.c_str();

            int length = strlen(message);

            cout << "result: " << result;

            ssize_t sent = send(sockfd, message, length, res->ai_flags);
            cout << "Client: " << result << endl;

            // receive message from server
            char buf[512];

            ssize_t recvServer = recv(sockfd, buf, sizeof(buf), res->ai_flags);
            cout << "Server: " << buf << endl;
        }
    }
}

void Client::closeSocket() {
    int status = 0;

#ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0) { status = closesocket(sock); }
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0) { status = close(sock); }
#endif
}

Client::Client(SOCKET sock) : sock(sock) {

}


