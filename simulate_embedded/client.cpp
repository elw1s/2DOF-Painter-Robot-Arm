#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int totalInt = 0, totalStr = 0;

void decodeMessage(const std::string &encodedMessage) {
    if (encodedMessage.empty()) {
        std::cout << "Received empty message" << std::endl;
        return;
    }

    std::cout << "Received MESSAGE ===>" << encodedMessage << std::endl;

    if (encodedMessage[0] == '0') {
        try {
            int integerValue = std::stoi(encodedMessage.substr(1));
            totalInt += 1;
            std::cout << "Received integer value: " << integerValue << " num: " << totalInt << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error converting string to integer: " << e.what() << std::endl;
        }
    } else if (encodedMessage[0] == '1') {
        std::string stringValue = encodedMessage.substr(1);
        totalStr += 1;
        std::cout << "Received string value: " << stringValue << " num: " << totalStr << std::endl;
    } else {
        std::cout << "Unknown message format" << std::endl;
    }
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Connect to server1
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Use the appropriate port number
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with server's IP

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection to server failed" << std::endl;
        close(clientSocket);
        return 1;
    }


    //send message
    char buffer[1024];
    int bytesRead;
    printf("Connection established!\n");
    std::string sentMessage = "0MerhabafromClient :)";
    send(clientSocket, sentMessage.c_str(), strlen(sentMessage.c_str()), 0);
    printf("First message is sent.");
    while(true){
        printf("Waiting for data from server!\n");
        if((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0){
            buffer[bytesRead] = '\0';
            std::string receivedMessage(buffer);
            decodeMessage(receivedMessage);
        }

        //send
        usleep(2000000); // Sleep for 2 seconds (2 million microseconds)
        sentMessage = std::to_string(totalInt) + " totalint";
        send(clientSocket, sentMessage.c_str(), strlen(sentMessage.c_str()), 0);
        printf("An answer is sent!\n");
    }

    close(clientSocket);
    return 0;
}
