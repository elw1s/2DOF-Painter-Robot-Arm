#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <thread>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <vector>
#include <algorithm> // Include the algorithm header for std::find
#include <bitset>    // Include for std::bitset
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

int numberOfLines = 0;   // Global variable to store the number of lines
std::vector<std::string> lines;
 
void send_message_to_client(int clientSocket, const char* jsonData, size_t dataSize) {
    std::cout << "Trying to send" << ": " << jsonData << std::endl;
    if (send(clientSocket, jsonData, dataSize, 0) < 0) {
        std::cerr << "Failed to send data\n";
    }
}

int line_number_command(int clientSocket, int serverSocket) {
    std::string lines = "0" + std::to_string(numberOfLines); // '0' + numberOfLines

    send_message_to_client(clientSocket, lines.c_str(), lines.size());

    return 0;
}

int drawn_line_command(int clientSocket, int serverSocket){
    std::string line = lines.at(0);
    line = '1' + line;
    send_message_to_client(clientSocket, line.c_str(), line.size());
    lines.erase(lines.begin());
    return 0;
}

int sensor_data_command(int clientSocket, int serverSocket){
    int gyroData = rand() % 360; // Generating gyro sensor data between 0 and 359 degrees
    gyroData = '2' + gyroData; // DECODER

    std::string dataToSend = std::to_string(gyroData);
    send_message_to_client(clientSocket, dataToSend.c_str(), dataToSend.size());

    return 1;
}
//Send 1 if robot is in idle mode, 0 otherwise
int idle_mode_command(int clientSocket, int serverSocket){
    if (lines.empty()) {
        const char* message = "41"; // DECODER: 4 - No lines left in document
        if (send(clientSocket, message, strlen(message), 0) < 0) {
            std::cerr << "Failed to send data\n";
        }
    } else {
        const char* message = "40"; // DECODER: 4 - Lines available in document
        if (send(clientSocket, message, strlen(message), 0) < 0) {
            std::cerr << "Failed to send data\n";
        }
    }
    return 0;
}
int servo_angles_command(int clientSocket, int serverSocket){
    std::vector<int> angles;
    for (int i = 0; i < 3; ++i) {
        int angle = rand() % 361 - 180; // Generate angles between -180 and +180 degrees
        angles.push_back(angle);
    }

    std::bitset<16> angle1(angles[0]);
    std::bitset<16> angle2(angles[1]);
    std::bitset<16> angle3(angles[2]);

    std::cout << "Created Angle 1:" << angle1.to_string() << std::endl;
    std::cout << "Created Angle 2:" << angle2.to_string() << std::endl;
    std::cout << "Created Angle 3:" << angle3.to_string() << std::endl;


    std::string dataToSend = '3' + angle1.to_string() + angle2.to_string() + angle3.to_string(); // DECODER + 3 angles
    send_message_to_client(clientSocket, dataToSend.c_str(), dataToSend.size());

    return 1;
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port_number> <file_path>\n";
        return 1;
    }

    int port = std::stoi(argv[1]); // Convert the argument to an integer
    const char* filePath = argv[2]; // Retrieve the file path from command line
    std::cout << filePath << std::endl;
    srand(time(nullptr)); // Seed the random number generator

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

    while (true) {
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

        int pid = fork(); // Create a child process for each client
        if (pid < 0) {
            std::cerr << "Error in forking\n";
            close(clientSocket);
            continue;
        } else if (pid == 0) { // Child process
            int randomCommand;
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
                    lines.push_back(buffer.GetString());
                }
            }

            numberOfLines = lines.size();
            int test = 0;

            while(true){
                randomCommand = rand() % 5;
                if(test == 0){
                    randomCommand = 0;
                    test++;
                }
                else if(randomCommand == 0){
                    continue;
                }
                switch (randomCommand) {
                    case 0:
                        line_number_command(clientSocket, serverSocket);
                        break;
                    case 1:
                        drawn_line_command(clientSocket, serverSocket);
                        break;
                    case 2:
                        sensor_data_command(clientSocket, serverSocket);
                        break;
                    case 3:
                        idle_mode_command(clientSocket, serverSocket);
                        break;
                    case 4:
                        servo_angles_command(clientSocket, serverSocket);
                        break;
                    default:
                        break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Adjust the delay time as needed
            }

            delete[] jsonData;

            close(clientSocket);
            return 0;
        } else { // Parent process
            close(clientSocket); // Close the client socket in the parent process
        }

    }
    
    close(serverSocket); // Close the server socket

    return 0;
}
