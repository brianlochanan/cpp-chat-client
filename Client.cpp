//
// Brian Lochan, Kubra Kasirga
//

#include <sys/proc_info.h>
#include "Client.h"

#define PORT "5378"
#define SERVER_IP "52.58.97.202"

using namespace std;

// declare variables
bool firsTime = true;
string command;
struct addrinfo hints, *res;
struct addrinfo *info;
string check = "0";
string username;
int sockfd;
string result;
CircularLineBuffer* stdinBuffer = new CircularLineBuffer();
CircularLineBuffer* socketBuffer = new CircularLineBuffer();
string serverRead;

int Client::tick() {

    if(stdinBuffer.hasLine()) {
        string command = stdinBuffer.readLine();
        sendToServer(command, sockfd, res);
        cout << command;
    }

    if(socketBuffer.hasLine()){
        string command = socketBuffer.readLine();
        cout << "socketBuffer.readLine():" << command << endl;
    }

    return 0;

}

int Client::readFromSocket() {
    string receiveServer = recvFromServer(sockfd, res, username);
    const char *message = receiveServer.c_str();

    cout << "writing to socketBuffer.." << endl;
    socketBuffer.writeChars(message, strlen(message));
    return 0;
}

int Client::readFromStdin() {
    if(firsTime){
        cout << "Type a username: " << endl;
        command = "";
        getline(cin, command);
        username = command;

        char *hellofromMessage = "HELLO-FROM ";

        // check if typed "HELLO-FROM" in username
        if (username.find(hellofromMessage) != std::string::npos) {
            result = username + '\n';
        }

        else{
            result = hellofromMessage + username + "\n";
        }

        firsTime = false;
    }
    else{
        cout << "Type a command: " << endl;
        command = "";
        getline(cin, command);
        command = command;
    }

    if (command == "!who") {
        result = "WHO\n";
    }
    else if (command.at(0) == '@') {
        // remove first character '@' and concatenate everything together
        result = "SEND " + command.substr(1) + "\n";
    }

    // stop the application
    else if (command == "!quit") {
        // after connection free up memory
        freeaddrinfo(res);
        //close socket
        sock_close(sockfd);
        sock_quit();
        this->stopApplication();
        return -1;
    }

    const char *message = result.c_str();
    cout << "writing to stdinBuffer.." << endl;
    stdinBuffer.writeChars(message, strlen(message));


    return 0;
}

void Client::createSocketAndLogIn() {

    // initialize socket
    sock_init();

    // set hints on 0
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    // get address of server
    int status = getaddrinfo(SERVER_IP, PORT, &hints, &res);

        // create socket
        sockfd = sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (!sock_valid(sockfd)) {
            cout << "Creating socket failed" << endl;
        };

        // connect socket with server
        connectToServer(sockfd, res);
}


bool Client::sendToServer(string result, int sockfd, addrinfo *res) {

    // make a const char of the string, because send function expects a char as message
    const char *message = result.c_str();

    int length = strlen(message);
    // send client message to server
    int test = send(sockfd, message, length, res->ai_flags);

    return true;
}

/**
 * Client sends a message and receives one back from the server
 * @param result
 * @param sockfd
 * @param res
 */
string Client::recvFromServer(int sockfd, addrinfo *res, string username) {
    // receive message from server
    char buf[512];

    recv(sockfd, buf, sizeof(buf), 0);

    string result;
    if (strncmp("IN-USE\n", buf, 6) == 0) {
        cout << "Username \"" << username << "\" in use. Choose another username. " << endl;
        return "";
    }

    else if (strncmp("BAD-RQST-BODY", buf, 6) == 0) {
        cout << "Username \"" << username << "\" bad request body." << endl;
        return "";
    }

    else if(strncmp("UNKNOWN", buf, 6) == 0) {
        cout << "Unknown username. try a valid username." << endl;
    }

    else if (strncmp("SEND-OK", buf, 6) == 0){
//        cout << "Server: " << buf << endl;
        recv(sockfd, buf, sizeof(buf), res->ai_flags);

        string receive;

        cout << "Would you like to receive the message back that you sent? y/n" << endl;
        getline(cin, receive);

        if(receive == "y"){
            cout << "Server: " << buf << endl;
        }
        result = buf;
    }

    else {
        result = buf;
//        cout << "Client: " << result << endl;
//        cout << "Server: " << buf << endl;
    }

    return result;
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


