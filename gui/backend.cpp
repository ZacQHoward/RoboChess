#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

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

    //Reading and creating web socket
    read(socket, buffer, 1024);
    std::cout << "Message Recieved " << buffer << std::endl;
    std::string strBuffer(buffer);
    if(strBuffer.find("upgrade")){
        std::cout << "upgrading connection" << std::endl;
        int acceptCodePos = strBuffer.find("Sec-WebSocket-Key");
        int len = 24;
        std::string acceptCodeKey = strBuffer.substr(acceptCodePos+19, len) + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        //base64 section can be moved to a function if needed elsewhere
        char obuf[20];
        BIO *bmem = nullptr;
        BIO *b64 = nullptr;
        BUF_MEM *bptr = nullptr;
        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bmem = BIO_new(BIO_s_mem());
        b64 = BIO_push(b64, bmem);
        unsigned char* acceptCode = SHA1((const unsigned char*)acceptCodeKey.c_str(), strlen(acceptCodeKey.c_str()), (unsigned char*)obuf);
        BIO_write(b64, acceptCode, 20);
        BIO_flush(b64);
        BIO_get_mem_ptr(b64, &bptr);
        std::string test(bptr->data, bptr->length);
        BIO_free_all(b64);
        std::string header = 
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: " + test + "\r\n"
            "\r\n";
        send(socket, header.c_str(), header.length(), 0);
    }
    close(socket);
    close(server);
    return 0;
};
