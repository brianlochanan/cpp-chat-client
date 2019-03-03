//
// Brian Lochan, Kubra Kasirga
//

#include <sys/proc_info.h>
#include "Client.h"

#define PORT "5378"
#define SERVER_IP "52.58.97.202"

using namespace std;

// declare variables
string command;
struct addrinfo hints, *res;
struct addrinfo *info;
bool check = false;
string username;
int sockfd;
string result;



int Client::tick() {

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
        return -1;
    }

    else {
        return 0;
    }
}

int Client::readFromStdin() {
    return 0;
}

int Client::readFromSocket() {
    return 0;
}

void Client::createSocketAndLogIn() {


    //Circular buffer example
    CircularLineBuffer* buffer = new CircularLineBuffer();

    cout << "test empty: " << buffer->isEmpty() << endl;
    const char *cstr = "HELLO\n"; //5
    bool test = buffer->writeChars(cstr, strlen(cstr));

    if(test){
        cout << "testhallo" << endl;
    }

    const char *cstr2 = "TEST\n"; //10
    if(buffer->writeChars(cstr2, strlen(cstr2))){
        cout << "gfhddfhdf";
    }

    const char *cstr3 = "BRIAN\n"; //10
    if(buffer->writeChars(cstr3, strlen(cstr3))){
        cout << "gfhddfhdf2";
    }

//    const char *cstr4 = "HI\n"; //10
//    if(buffer->writeChars(cstr4, strlen(cstr4))){
//        cout << "gfhddfhdf2";
//    }

    cout << "buffer->findNewline(): " << buffer->findNewline() << endl;

    cout << "buffer->hasLine(): " << buffer->hasLine() << endl;

    cout << "test read from buffer: " << buffer->readLine() << endl;

    cout << "test read from buffer: " << buffer->readLine() << endl;



//----------------------------------------------------------------------------------------------------------------------

    // initialize socket
    sock_init();

    // set hints on 0
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    // get address of server
    int status = getaddrinfo(SERVER_IP, PORT, &hints, &res);

    // set message for sending to server
    char *message = "HELLO-FROM ";

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
        cout << "Username \"" << username << "\" bad request body." << endl;
        return false;
    }

    else if(strncmp("UNKNOWN", buf, 6) == 0) {
        cout << "Unknown username. try a valid username." << endl;
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


