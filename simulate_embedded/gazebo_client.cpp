#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>

struct PositionCommand {
    double joint1;
    double joint2;
    double joint3;
};

int PORT = 8081;
char* IP_ADDRESS = "127.0.0.1"; // default values
int sock = 0;
struct sockaddr_in serv_addr;
void handle_sigint(int);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip_address> <port>\n" << std::endl;
    } else {
        IP_ADDRESS = argv[1];
        PORT = std::atoi(argv[2]);
    }

    std::ofstream outputFile("/home/arda/Desktop/CSE396/simulate_embedded/gazebo_output.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the file" << std::endl;
        return -1;
    }

    outputFile << "PROCESS IS STARTED" << std::endl;
    outputFile << "Starting at " << IP_ADDRESS << ":" << PORT << std::endl;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        outputFile << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr) <= 0) {
        outputFile << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        outputFile << "Binding failed" << std::endl;
        return -1;
    }

    if (listen(sock, 1) < 0) {
        outputFile << "Listening error" << std::endl;
        return -1;
    }

    int clientSock;
    socklen_t clientSize;
    struct sockaddr_in client_addr;

    outputFile << "Waiting for a client to connect..." << std::endl;

    clientSock = accept(sock, (struct sockaddr *)&client_addr, &clientSize);

    if (clientSock < 0) {
        outputFile << "Error accepting client connection" << std::endl;
        return -1;
    }

    outputFile << "Client connected" << std::endl;

    signal(SIGINT, handle_sigint);

    PositionCommand positionCommand;
    char receivedData[sizeof(PositionCommand)];

    while (true) {
        ssize_t bytesRead = read(clientSock, &receivedData, sizeof(receivedData));

        if (bytesRead == sizeof(PositionCommand)) {
            PositionCommand receivedPositionCommand;

            // Deserialize the received byte array into a PositionCommand struct
            memcpy(&receivedPositionCommand, &receivedData, sizeof(receivedPositionCommand));

            // Access the received data
            outputFile << "Received PositionCommand: "
                       << receivedPositionCommand.joint1 << ", "
                       << receivedPositionCommand.joint2 << ", "
                       << receivedPositionCommand.joint3 << std::endl;

        } else {
            // Handle incomplete data or error in receiving
            std::cerr << "Incomplete data or error in receiving" << std::endl;
            break;
        }
    }

    outputFile.close();
    return 0;
}

void handle_sigint(int sig) {
    close(sock);
    exit(0);
}

