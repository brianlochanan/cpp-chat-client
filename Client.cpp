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
    while (true){

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
            exit(1);
        }

        else{
            // send message to server
            string result;

            // gets who is online
            if(str == "!who"){
                // add number at the end of the client string, because then you can call !who again
                std::string number = std::to_string(counter++);
                result = "HELLO-FROM testClient" + number + "\n";
                recvFromServer(result, sockfd, res);

                result = "WHO\n";
                recvFromServer(result, sockfd, res);
            }

            // make unique user
            else{
                char *message = "HELLO-FROM ";
                result = message + str + "\n";
                recvFromServer(result, sockfd, res);
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
void Client::recvFromServer(string result, int sockfd, addrinfo *res){
    // make a const char of the string, because send function expects a char as message
    const char *message = result.c_str();

    int length = strlen(message);

    // send client message to server
    send(sockfd, message, length, res->ai_flags);
    cout << "Client: " << result << endl;

    // receive message from server
    char buf[512];
    recv(sockfd, buf, sizeof(buf), res->ai_flags);
    cout << "Server: " << buf << endl;
}


Client::Client(SOCKET sock) : sock(sock) {

}


