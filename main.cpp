#include <iostream>
#include "Client.h"

int main(int argc, char *argv[ ]) {
    struct hostent *h;

    std::cout << "Computer Networks Chat Client Starting..." << std::endl;
    Application* app = new Client(1);
    std::cout << "Setting up" << std::endl;
    app->setup();
    std::cout << "Running application" << std::endl;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(1);
    }

    if((h=gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname(): ");
        exit(1);
    }
    else{
        printf("Hostname: %s\n", h->h_name);
        printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));
    }

    app->run();

    delete app;
    return 0;
}
