#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int BUFFER_SIZE = 4096;

void handleError(const std::string& msg) {
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    const std::string host = "127.0.0.1";
    const int port = 8080;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        handleError("socket");
    }

    // Setup server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        handleError("connect");
    }

    // Send an HTTP GET request
    const std::string request = 
        "GET / HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Connection: close\r\n"
        "\r\n";

    if (send(sockfd, request.c_str(), request.size(), 0) < 0) {
        handleError("send");
    }

    // Receive the server's response
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytesReceived = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived < 0) {
        handleError("recv");
    }

    // Print the response
    std::cout << "Received " << bytesReceived << " bytes:\n" << buffer << std::endl;

    // Close the socket
    close(sockfd);

    return 0;
}
