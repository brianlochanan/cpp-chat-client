//
// Brian Lochan, Kubra Kasirga
//

#include <sys/proc_info.h>
#include "Client.h"

#define PORT "5378"
#define SERVER_IP "52.58.97.202"

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
    // initialize socket
    sock_init();

    // declare variables
    struct addrinfo hints, *res;
    struct addrinfo *info;

    // set hints on 0
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    // get address of server
    int status = getaddrinfo(SERVER_IP, PORT, &hints, &res);

    string result;

    // set message for sending to server
    char *message = "HELLO-FROM ";

    // values needed for first handshake
    bool check = false;
    string username;
    int sockfd;

    // create socket in while loop, because new connection must be made when something
    // faulty is sent as username e.g.: "!who"
    while (!check) {
        // create socket
        sockfd = sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (!sock_valid(sockfd)) {
            cout << "Creating socket failed" << endl;
        };

        // connect socket with server
        connectToServer(sockfd, res);

        cout << "Input your name: " << endl;
        getline(cin, username);

        // check if typed "HELLO-FROM" in username
        if (username.find(message) != std::string::npos) {
            result = username + "\n";
        }

        else{
            result = message + username + "\n";
        }

        // check if login to server is successful
        check = recvFromServer(result, sockfd, res, username);
    }

    // send command to server
    while (true) {
        string command;
        cout << "Type a command: " << endl;

        getline(cin, command);

        // see who is online
        if (command == "!who") {
            string who = "WHO\n";
            recvFromServer(who, sockfd, res, username);
        }

        // send message to user
        else if (command.at(0) == '@') {
            // remove first character '@' and concatenate everything together
            result = "SEND " + command.substr(1) + "\n";

            recvFromServer(result, sockfd, res, username);
        }

        // stop the application
        else if (command == "!quit") {
            // after connection free up memory
            freeaddrinfo(res);
            //close socket
            sock_close(sockfd);
            sock_quit();
            this->stopApplication();
            exit(1);
        }
    }

}

/**
 * Client sends a message and receives one back from the server
 * @param result
 * @param sockfd
 * @param res
 */
bool Client::recvFromServer(string result, int sockfd, addrinfo *res, string username) {
    // make a const char of the string, because send function expects a char as message
    const char *message = result.c_str();

    int length = strlen(message);

    // send client message to server
    send(sockfd, message, length, res->ai_flags);

    // receive message from server
    char buf[512];
    recv(sockfd, buf, sizeof(buf), res->ai_flags);

    if (strncmp("IN-USE\n", buf, 6) == 0) {
        cout << "Username \"" << username << "\" in use. Choose another username. " << endl;
        return false;
    }

    else if (strncmp("BAD-RQST-BODY", buf, 6) == 0) {
        cout << "Username \"" << username << "\" failed. Choose another username. " << endl;
        return false;
    }

    else if (strncmp("SEND-OK", buf, 6) == 0){
        cout << "Server: " << buf << endl;
        recv(sockfd, buf, sizeof(buf), res->ai_flags);

        string receive;

        cout << "Would you like to receive the message back that you sent? y/n" << endl;
        getline(cin, receive);

        if(receive == "y"){
            cout << "Server: " << buf << endl;
        }
    }

    else {
        cout << "Client: " << result << endl;
        cout << "Server: " << buf << endl;
        return true;
    }
}

/**
 * @param sockfd
 * @param res
 */
void Client::connectToServer(int sockfd, addrinfo *res) {
    int conn = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (conn <= -1) {
        cout << "Connection to server failed" << endl;
    }
}

void Client::closeSocket() {

}


Client::Client(SOCKET sock) : sock(sock) {

}


