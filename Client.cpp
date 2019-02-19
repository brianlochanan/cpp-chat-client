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

    sock_init(); // initialize socket

    // declare variables
    int status;
    struct addrinfo hints, *res;
    struct addrinfo *info;

    // set hints on 0
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    //get address of server
    status = getaddrinfo(SERVER_IP, PORT, &hints, &res);

    int counter = 0;

        // create socket
        int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (!sock_valid(sockfd)) {
            cout << "Creating socket failed" << endl;
        };

        // connect socket with server
        int conn = connect(sockfd, res->ai_addr, res->ai_addrlen);

        // user input for sending client message
        string str;
        cout << "Input your name: " << endl;
        cin >> str;

        if (str == "!quit"){
            // after connection free up memory
            freeaddrinfo(res);
            //close socket
            sock_close(sockfd);
            sock_quit();
            Application::stopApplication();
        }

        else{
            // send message to server
            string result;

            // make unique user
                char *message = "HELLO-FROM ";
                result = message + str + "\n";
                bool check = recvFromServer(result, sockfd, res, str);
                if(!check) {
                    cout << "Input your name: " << endl;
                    cin >> str;
                    result = message + str + "\n";
                    recvFromServer(result, sockfd, res, str);
                }

            else {

                while (true) {
                    // gets who is online
                    string command;
                    cout << "Type a command: " << endl;
                    cin >> command;
//                    getline(cin, command);

                    if(command == "!who"){
                        string who = "WHO\n";
                        recvFromServer(who, sockfd, res, str);
                    }
                    else  {
                        // after connection free up memory
                        freeaddrinfo(res);
                        //close socket
                        sock_close(sockfd);
                        sock_quit();
                        Application::stopApplication();
                    }
            }
            }

        }
}

/**
 * Client sends a message and receives one back from the server
 * @param result
 * @param sockfd
 * @param res
 */
bool Client::recvFromServer(string result, int sockfd, addrinfo *res, string str){
    // make a const char of the string, because send function expects a char as message
    const char *message = result.c_str();

    int length = strlen(message);

    // send client message to server
    send(sockfd, message, length, res->ai_flags);
    cout << "Client: " << result << endl;

    // receive message from server
    char buf[512];
    recv(sockfd, buf, sizeof(buf), res->ai_flags);

    if (strncmp("IN-USE\n", buf, 6) == 0) {
        cout << "Username \"" << str <<  "\" in use. Choose another username. "<< endl;
        return false;
    }
    else {
        cout << "Server: " << buf << endl;
        return true;
    }
}

void Client::closeSocket() {

}


Client::Client(SOCKET sock) : sock(sock) {

}


