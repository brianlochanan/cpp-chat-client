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

    //get address of server
    status = getaddrinfo("52.58.97.202", "5378", &hints, &res);

    char *message;
    while (true){

        // create socket
        int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (!sock_valid(sockfd)) {
            cout << "Creating socket failed" << endl;
        };

        // connect socket with server
        int conn = connect(sockfd, res->ai_addr, res->ai_addrlen);

        // input name
        string str;
        cout << "Input your name: " << endl;
        cin >> str;

        if (str == "!quit"){
            // after connection free up memory
            freeaddrinfo(res);

            //close socket
            sock_close(sockfd);
            sock_quit();
            exit(1);
        }

        else{
            // send message to server
            string result;
            if(str == "!who"){
                result = "HELLO-FROM testServer\n";
                recvFromServer(result, sockfd, res);

                result = "WHO\n";
                recvFromServer(result, sockfd, res);
            }
            else{
                message = "HELLO-FROM ";
                result = message + str + "\n";
                recvFromServer(result, sockfd, res);
            }
        }
    }
}


void Client::recvFromServer(string result, int sockfd, addrinfo *res){
    const char *message = result.c_str();

    int length = strlen(message);

    send(sockfd, message, length, res->ai_flags);
    cout << "Client: " << result << endl;

    // receive message from server
    char buf[512];
    recv(sockfd, buf, sizeof(buf), res->ai_flags);
    cout << "Server: " << buf << endl;
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


