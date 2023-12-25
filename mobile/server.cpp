#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <fstream>

#define PORT 8080
#define MAX_IMAGE_SIZE 10000000 // Maximum image size
#define BUFFER_SIZE 4096

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::vector<unsigned char> imageBuffer; // Vector to hold image data

    // Receive image data until the connection closes
    int bytesRead;
    unsigned char buffer[BUFFER_SIZE];
    int totalBytesReceived = 0;

    do {
        bytesRead = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytesRead < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        } else if (bytesRead > 0) {
            totalBytesReceived += bytesRead;

            // Determine packet type
            unsigned char packetType = buffer[0];

            // Append data to imageBuffer excluding the first byte (packet type)
            imageBuffer.insert(imageBuffer.end(), buffer + 1, buffer + bytesRead);

            std::cout << "Received package type: " << packetType << std::endl;
            std::cout << "Received data: ";
            for (size_t i = 1; i < bytesRead; ++i) {
                std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::endl;
            
            // Check if this is the last packet
            if (static_cast<char>(packetType) == 2 || static_cast<char>(packetType) == '2') {
                // Image transfer complete, write to file or process as needed
                std::cout << "Image transfer complete, write to file or process as needed" << std::endl;
                std::cout << "Total received bytes: " << imageBuffer.size() << std::endl;
                std::ofstream outfile("received_image.jpg", std::ios::out | std::ios::binary);
                outfile.write(reinterpret_cast<char*>(&imageBuffer[0]), imageBuffer.size());
                outfile.close();

                break; // Exit loop
            }
        }
    } while (bytesRead > 0 && totalBytesReceived < MAX_IMAGE_SIZE);

    close(new_socket);
    close(server_fd);

    return 0;
}