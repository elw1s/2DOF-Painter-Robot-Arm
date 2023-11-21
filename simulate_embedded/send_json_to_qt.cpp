#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <thread>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

void sendJsonData(int clientSocket, const char* jsonData, size_t dataSize) {
    std::cout << "Trying to send" << ": " << jsonData << std::endl;
    if (send(clientSocket, jsonData, dataSize, 0) < 0) {
        std::cerr << "Failed to send data\n";
    }
}

int main() {
    const char* filePath = "/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json";
    int port = 12346;

    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Set up server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Binding failed\n";
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listen failed\n";
        close(serverSocket);
        return -1;
    }

    // Accept incoming connection
    int clientSocket;
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        std::cerr << "Error accepting connection\n";
        close(serverSocket);
        return -1;
    }

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        close(clientSocket);
        close(serverSocket);
        return -1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* jsonData = new char[size];
    if (!file.read(jsonData, size)) {
        std::cerr << "Failed to read file\n";
        file.close();
        close(clientSocket);
        close(serverSocket);
        delete[] jsonData;
        return -1;
    }
    file.close();

    rapidjson::Document doc;
    doc.Parse(jsonData);

    if (doc.HasParseError()) {
        std::cerr << "JSON parse error\n";
        close(clientSocket);
        close(serverSocket);
        delete[] jsonData;
        return -1;
    }

    // Check if it's an array of arrays (2D array)
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
        if (doc[i].IsArray()) {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc[i].Accept(writer);

            std::cout << "Parsed " << i + 1 << ": " << buffer.GetString() << std::endl;

            // Sending each parsed part to the client
            sendJsonData(clientSocket, buffer.GetString(), buffer.GetSize());

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    delete[] jsonData;
    close(clientSocket); // Close the client socket
    close(serverSocket); // Close the server socket

    return 0;
}
