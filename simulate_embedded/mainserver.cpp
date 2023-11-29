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

std::queue<std::string> messagesWaitingToBeSend;
char globalDataRecv[4096] = ""; // Global variable for data
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t canSendMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t canSendCond = PTHREAD_COND_INITIALIZER;

bool canSend = false;

void* communicationThread(void* clientSocket){
    int socketDescriptor = *((int*)clientSocket);


    while(1){
        
        printf("mutexLock before recv()\n");
        pthread_mutex_lock(&dataMutex);
        printf("mutexLock happened\n");
        int bytesRead = recv(socketDescriptor, globalDataRecv, sizeof(globalDataRecv), 0);
        printf("~~~~~~~~~~~~~ THE DATA IS RECEIVED ~~~~~~~~~~~~~\n");
        printf("Data (%s) received from client!\n", globalDataRecv);
        if (bytesRead <= 0) {
            // Handle disconnection or error
            //break;
            printf("Nothing comes...\n");
        }
        globalDataRecv[bytesRead] = '\0';
        pthread_mutex_unlock(&dataMutex);

        pthread_mutex_lock(&dataMutex);
        printf("data mutex locked\n");
        while(messagesWaitingToBeSend.size() == 0){
            printf("there is no message to sent\n");
            pthread_cond_wait(&dataCond, &dataMutex);
            printf("after pthread_cond_wait %ld\n", messagesWaitingToBeSend.size());
        }
        printf("before send %ld\n", messagesWaitingToBeSend.size());
        send(socketDescriptor, messagesWaitingToBeSend.front().c_str(), strlen(messagesWaitingToBeSend.front().c_str()), 0);
        printf("-------------- THE DATA IS SENT ---------------\n");
        printf("Gönderilen data: %s\n",messagesWaitingToBeSend.front().c_str());
        messagesWaitingToBeSend.pop();
        printf("Bekleyen mesaj sayisi: %ld\n",messagesWaitingToBeSend.size());
        pthread_mutex_unlock(&dataMutex);
        printf("Data mutex unlocked\n");

    }
}

// Function for thread handling server1
void* server1Thread(void* arg) {
    printf("Thread for server1\n");
    while(true){
        server1(messagesWaitingToBeSend, &dataCond, &dataMutex); 
    }
    return NULL;
}

// Function for thread handling server2
void* server2Thread(void* arg) {
    printf("Thread for server2\n");
    readLines("/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json",messagesWaitingToBeSend,&dataCond, &dataMutex);
    int lineNum = getLineNumber();
    printf("Line number: %d\n",lineNum);
    sendLineNumber(messagesWaitingToBeSend, &dataCond, &dataMutex);
    while(true){
     server2(messagesWaitingToBeSend, &dataCond, &dataMutex);   
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
