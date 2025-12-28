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
        //This gives time for the request to come in and should be improved
        //TODO: Fix
        sleep(1);

        char JSONbuffer[2048] = {0};
        read(socket, JSONbuffer, 2048);
        
        // Decode WebSocket frame
        unsigned char* frame = (unsigned char*)JSONbuffer;
        int payloadLen = frame[1] & 0x7F; // Remove mask bit
        int maskStart = 2;
        
        // Handle extended payload lengths
        if (payloadLen == 126) {
            payloadLen = (frame[2] << 8) | frame[3];
            maskStart = 4;
        } else if (payloadLen == 127) {
            maskStart = 10; // 8 bytes for length + 2 header bytes
        }
        
        // Extract masking key (4 bytes after length)
        unsigned char mask[4];
        for (int i = 0; i < 4; i++) {
            mask[i] = frame[maskStart + i];
        }
        
        // Unmask payload
        int payloadStart = maskStart + 4;
        char decodedJSON[2048] = {0};
        for (int i = 0; i < payloadLen; i++) {
            decodedJSON[i] = frame[payloadStart + i] ^ mask[i % 4];
        }
        
        std::cout << decodedJSON << std::endl;
        //TESTING TODO: Remove
        std::string testResponse = R"({"WRRook":["A1","images/wR.svg"],"WRKnight":["B1","images/wN.svg"],"WRBishop":["C1","images/wB.svg"],"WQueen":["D1","images/wQ.svg"],"WKing":["E1","images/wK.svg"],"WLBishop":["F1","images/wB.svg"],"WLKnight":["G1","images/wN.svg"],"WLRook":["H1","images/wR.svg"],"WAPawn":["A2","images/wP.svg"],"WBPawn":["B2","images/wP.svg"],"WCPawn":["C2","images/wP.svg"],"WDPawn":["D2","images/wP.svg"],"WEPawn":["E2","images/wP.svg"],"WFPawn":["F2","images/wP.svg"],"WGPawn":["G2","images/wP.svg"],"WHPawn":["H2","images/wP.svg"],"BRRook":["A8","images/bR.svg"],"BRKnight":["B8","images/bN.svg"],"BRBishop":["C8","images/bB.svg"],"BQueen":["D8","images/bQ.svg"],"BKing":["E8","images/bK.svg"],"BLBishop":["F8","images/bB.svg"],"BLKnight":["G8","images/bN.svg"],"BLRook":["H8","images/bR.svg"],"BAPawn":["A7","images/bP.svg"],"BBPawn":["B7","images/bP.svg"],"BCPawn":["C7","images/bP.svg"],"BDPawn":["D7","images/bP.svg"],"BEPawn":["E7","images/bP.svg"],"BFPawn":["F7","images/bP.svg"],"BGPawn":["G7","images/bP.svg"],"BHPawn":["H6","images/bP.svg"]})";
        
        // Frame the response for WebSocket
        int responseLen = testResponse.length();
        unsigned char responseFrame[2048];
        int frameSize = 0;
        
        // Byte 0: FIN bit (1) + opcode (1 = text frame)
        responseFrame[frameSize++] = 0x81;
        
        // Byte 1: Mask bit (0 for server->client) + payload length
        if (responseLen < 126) {
            responseFrame[frameSize++] = responseLen;
        } else if (responseLen < 65536) {
            responseFrame[frameSize++] = 126;
            responseFrame[frameSize++] = (responseLen >> 8) & 0xFF;
            responseFrame[frameSize++] = responseLen & 0xFF;
        }
        
        // Copy payload
        memcpy(responseFrame + frameSize, testResponse.c_str(), responseLen);
        frameSize += responseLen;
        
        send(socket, responseFrame, frameSize, 0);
        sleep(10);
    }
    close(socket);
    close(server);
    return 0;
};
