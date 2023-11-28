#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include "server_code1.h"
#include "server_code2.h"

char globalDataSend[1024] = ""; // Global variable for data
char globalDataRecv[1024] = ""; // Global variable for data
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t canSendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t canSendCond = PTHREAD_COND_INITIALIZER;

bool canSend = false;

void* dataSenderThread(void* clientSocket) {
    int socketDescriptor = *((int*)clientSocket);

    while (1) {
        pthread_mutex_lock(&dataMutex);

        while (!canSend) {
            // Wait for receiver's signal to send data
            pthread_cond_wait(&canSendCond, &dataMutex);
        }

        // Send the modified data
        send(socketDescriptor, globalDataSend, strlen(globalDataSend), 0);
        printf("-------------- THE DATA IS SENT ---------------\n");

        // Notify the receiver that data is sent
        pthread_mutex_lock(&canSendMutex);
        canSend = false;
        pthread_cond_signal(&canSendCond); // Signal receiver to continue receiving
        pthread_mutex_unlock(&canSendMutex);

        pthread_mutex_unlock(&dataMutex);
    }
}

void* dataReceiverThread(void* clientSocket){
    int socketDescriptor = *((int*)clientSocket);
    //printf("Inside dataReceiverThread!\n");
    while(1){
        //printf("dataReceiver: Locking Mutex...\n");
        //printf("dataReceiver: Locked Mutex...\n");
        pthread_mutex_lock(&canSendMutex);
        while (canSend) {
            pthread_cond_wait(&canSendCond, &canSendMutex); // Wait for client signal to send
        }

        // Receive data from the client
        char receivedData[1024]; // Assuming max data size is 1024 bytes
        int bytesRead = recv(socketDescriptor, receivedData, sizeof(receivedData), 0);
        printf("~~~~~~~~~~~~~ THE DATA IS RECEIVED ~~~~~~~~~~~~~\n");
        printf("Data (%s) received from client!\n", receivedData);
        if (bytesRead <= 0) {
            // Handle disconnection or error
            //break;
            printf("Nothing comes...\n");
        }
        receivedData[bytesRead] = '\0';

        // Store received data into globalDataSend
        strcpy(globalDataRecv, receivedData);

        canSend = true;
        pthread_cond_signal(&canSendCond); // Signal client to send
        pthread_mutex_unlock(&canSendMutex);
    }
}

// Function for thread handling server1
void* server1Thread(void* arg) {
    printf("Thread for server1\n");
    while(true){
        server1(globalDataSend, &dataCond, &dataMutex); 
    }
    // Modify globalDataSend in server1
    // Signal the change in data
    //pthread_cond_signal(&dataCond);
    return NULL;
}

// Function for thread handling server2
void* server2Thread(void* arg) {
    printf("Thread for server2\n");
    readLines("/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json");
    sendLineNumber(globalDataSend, &dataCond, &dataMutex);
    while(true){
     server2(globalDataSend, &dataCond, &dataMutex);   
    }
    // Signal the change in data
    //pthread_cond_signal(&dataCond);
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


        // Create the thread for sending data
        pthread_t receiverThread;
        if (pthread_create(&receiverThread, NULL, dataReceiverThread, (void*)&clientSocket) != 0) {
            perror("Receiver thread creation failed");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        // Create the thread for sending data
        pthread_t senderThread;
        if (pthread_create(&senderThread, NULL, dataSenderThread, (void*)&clientSocket) != 0) {
            perror("Sender thread creation failed");
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
