#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include "server_code1.h"
#include <random>
#include <bitset>

int generateRandomNumber1() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-90, 90);
    return dis(gen);
}


void server1(char* message, pthread_cond_t* condition, pthread_mutex_t* mutex) {
    int total = 0;
    std::cout << "Server 1 function started for client\n";

    int dataToSend = generateRandomNumber1(); // Generating a random integer (-90 to 90)
    std::bitset<16> value(dataToSend);


    std::string encodedData = '2' + value.to_string(); // Creating the encoded string

    printf("SERVER1: Locking mutex...\n");
    pthread_mutex_lock(mutex); // Lock the mutex before modifying shared data
    printf("SERVER1: Locked mutex...\n");
    strcpy(message, encodedData.c_str());
    pthread_cond_signal(condition); // Signal the condition variable
    printf("SERVER1: Sent signal...\n");
    pthread_mutex_unlock(mutex); // Unlock the mutex
    printf("SERVER1: Unlocked mutex...\n");

    total += 1;
    std::cout << "Send: " << encodedData << " num:" << total << std::endl;

    // Introduce some delay to simulate work
    usleep(2000000); // Sleep for 2 seconds (2 million microseconds)
}
