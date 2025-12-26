#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main() {
    //Server creation fails with code 1
    int port = 8080;
    int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int opt = 1;
    if (server == 0) {
        std::cout << "server failed to create" << std::endl;
        exit(1);
    } 

    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    std::cout << "Server started on port " << port << std::endl;
    //Binding fails with code 2
    int openBind = bind(server, (struct sockaddr *)&address, addrlen);
    if(openBind < 0){
        std::cout << "socket failed to bind" << std::endl;
        exit(2);
    }

    //Listening (sets the max request queue to 5) fails with code 3
    int openListen = listen(server, 5);
    if(openListen < 0){
        std::cout << "socket failed to listen(too many requests?)" << openListen << std::endl;
        exit(3);
    }

    //Accepting connection fails with code 4
    int socket = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if(socket < 0){
        std::cout << "socket failed to listen(too many requests?)" << openListen << std::endl;
        exit(4);
    }

    //Reading
    read(socket, buffer, 1024);
    std::cout << "Message Recieved " << buffer << std::endl;
    close(socket);
    close(server);
    return 0;
};
