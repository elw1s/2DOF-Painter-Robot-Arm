#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include "server_code1.h"
#include "server_code2.h"
#include <queue>
#include "brachiograph.cpp"

#define MAX_DATA_SIZE 4096 // Maximum size for received image data
#define MAX_IMAGE_SIZE 10000000 // 10 MB


std::queue<std::string> messagesWaitingToBeSend;
uint8_t globalDataRecv[MAX_DATA_SIZE]; // Global variable for data
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t canSendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t canSendCond = PTHREAD_COND_INITIALIZER;

pthread_cond_t clientDisconnectedCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t server2Cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t case2Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clientDisconnectedMutex = PTHREAD_MUTEX_INITIALIZER;

bool case2Encountered = false;
bool clientDisconnected = false; // Flag to track client disconnection
bool canSend = false;
unsigned char imageBuffer[MAX_IMAGE_SIZE]; // Byte array to hold image data
size_t imageBufferIndex = 0; // Index to keep track of the image buffer position
BrachioGraph bg = BrachioGraph(messagesWaitingToBeSend,&dataCond,&dataMutex);

void connection_established(){
    pthread_mutex_lock(&dataMutex);
    messagesWaitingToBeSend.push("0Connection Established.");
    pthread_mutex_unlock(&dataMutex);
}

void image_received(){
    pthread_mutex_lock(&dataMutex);
    messagesWaitingToBeSend.push("1Image Received");
    pthread_mutex_unlock(&dataMutex);
}

// Function to handle case 1 (Image values)
void add_image_data_to_array(size_t dataSize) {
    // Append the received image data to the image buffer
    if (imageBufferIndex + dataSize < MAX_IMAGE_SIZE) {
        pthread_mutex_lock(&dataMutex);
        memcpy(imageBuffer + imageBufferIndex, globalDataRecv + 1, dataSize - 1); // Skip the first character and exclude null terminator
        pthread_mutex_unlock(&dataMutex);
        imageBufferIndex += dataSize - 1; // Update the image buffer index
        printf("Received image byte data appended to buffer. %ld byte\n",dataSize);
    } else {
        printf("Image buffer full. Cannot append more data.\n");
    }
}

void write_image_to_file() {
    FILE* imageFile = fopen("/tmp/cse396/sent.jpg", "wb");
    if (imageFile != NULL) {
        fwrite(imageBuffer, sizeof(unsigned char), imageBufferIndex, imageFile);
        fclose(imageFile);
        printf("Image data written to file successfully.\n");
    } else {
        printf("Error opening file to write image data.\n");
    }
}

void* communicationThread(void* clientSocket){
    int socketDescriptor = *((int*)clientSocket);


    while(1){
        
        pthread_mutex_lock(&dataMutex);
        int bytesRead = recv(socketDescriptor, globalDataRecv, sizeof(globalDataRecv), 0);
        if (bytesRead <= 0) {
	    // Handle disconnection or error
            printf("Client disconnected or error occurred.\n");
            close(socketDescriptor); // Close the socket
            pthread_mutex_unlock(&dataMutex);
            return NULL; // Exit the thread
        }
        else{            
            for (int i = 0; i < MAX_DATA_SIZE; ++i) {
                if (globalDataRecv[i] == '\0') {
                    break; // Stop printing when the null terminator is encountered
                }
            }            
            pthread_mutex_unlock(&dataMutex);
            switch (static_cast<char>(globalDataRecv[0]))
            {
            case '0': // Connection established
                connection_established();
                break;
            case '1': // Image values
                add_image_data_to_array(bytesRead);
                image_received();
                break;
            case '2': // End of image
                //Server1 threadi çalışabilir.
                //Server2 direkt bağlantı sağlanınca çalışabilir.
                add_image_data_to_array(bytesRead);
                write_image_to_file();
                imageBufferIndex = 0;
                pthread_mutex_lock(&case2Mutex);
                case2Encountered = true;
                //pthread_cond_signal(&server1Cond);
                pthread_cond_signal(&server2Cond);
                pthread_mutex_unlock(&case2Mutex);
                break;
            case '3': // Set servo angles
                printf("SET SERVO ANGLES\n");
                break;
            case '4': // Stop drawing
                printf("STOP DRAWING\n");
                break;
            case '5': // Cancel drawing
                printf("CANCEL DRAWING\n");
                break;
            default:
                break;
            }
        }    

        pthread_mutex_lock(&dataMutex);
        while(messagesWaitingToBeSend.size() == 0){
            pthread_cond_wait(&dataCond, &dataMutex);
        }

        send(socketDescriptor, messagesWaitingToBeSend.front().c_str(), strlen(messagesWaitingToBeSend.front().c_str()), 0);
        //printf("-------------- THE DATA IS SENT ---------------\n");
        //printf("Gönderilen data: %s\n",messagesWaitingToBeSend.front().c_str());
        messagesWaitingToBeSend.pop();
        //printf("Bekleyen mesaj sayisi: %ld\n",messagesWaitingToBeSend.size());
        pthread_mutex_unlock(&dataMutex);
    }
}

// SERVER1 THREADI, '2' KOMUTU GELMEDEN ÇALIŞMAYACAKTIR!!!

void* server1Thread(void* arg) {
    printf("Thread for server1\n");
    
    //pthread_mutex_lock(&case2Mutex);
    //while (!case2Encountered) {
    //    pthread_cond_wait(&server1Cond, &case2Mutex);
    //}
    //pthread_mutex_unlock(&case2Mutex);

    while(true){
        server1(messagesWaitingToBeSend, &dataCond, &dataMutex); 
    }
    return NULL;
}

// Function for thread handling server2
void* server2Thread(void* arg) {
    printf("Thread for server2\n");

    while(true){
        pthread_mutex_lock(&case2Mutex);
        while (!case2Encountered) {
            pthread_cond_wait(&server2Cond, &case2Mutex);
        }
        pthread_mutex_unlock(&case2Mutex);

        //BrachioGraph::imageToJson("/tmp/cse396/sent.jpg", 1024, 2, 1 , 16, 1);
        //Path değiştir...
        system("python3 /home/ardakilic/Desktop/CSE396/simulate_embedded/linedraw.py");
        usleep(2000000);
        bg.plot_file("/tmp/cse396/sent.json");
        //readLines("/tmp/cse396/sent.json",messagesWaitingToBeSend,&dataCond, &dataMutex);
        int lineNum = getLineNumber();
        printf("Line number: %d\n",lineNum);
        sendLineNumber(messagesWaitingToBeSend, &dataCond, &dataMutex);
        while(true){
            if(!server2(messagesWaitingToBeSend, &dataCond, &dataMutex))
                break;
        }
        printf("server2 started again...\n");
        case2Encountered = false;
    }
    return NULL;
}

int main() {
    // Create socket for server
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server details
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    serverAddr.sin_port = htons(8080); // Port to listen on

    // Bind the socket to the specified port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(serverSocket, SOMAXCONN) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening for incoming connections...\n");

    // Accept incoming connections and handle them in separate threads
    while (1) {
        struct sockaddr_in clientAddr;
        int clientSocket;
        socklen_t addrLen = sizeof(clientAddr);

        // Accept a new client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            perror("Acceptance failed");
            continue;
        }

        printf("New connection accepted\n");
	
	clientDisconnected = true;

        // Create the thread for sending and receiving data
        pthread_t communication_thread;
        if (pthread_create(&communication_thread, NULL, communicationThread, (void*)&clientSocket) != 0) {
            perror("Receiver thread creation failed");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }


        // Create threads for server1 and server2
        pthread_t thread1, thread2;

        // Thread for server1
        if (pthread_create(&thread1, NULL, server1Thread, NULL) != 0) {
            perror("Thread creation failed");
            close(clientSocket);
            continue;
        }

        // Thread for server2
        if (pthread_create(&thread2, NULL, server2Thread, NULL) != 0) {
            perror("Thread creation failed");
            close(clientSocket);
            continue;
        }

        // Detach the threads for server1 and server2
        pthread_detach(thread1);
        pthread_detach(thread2);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
