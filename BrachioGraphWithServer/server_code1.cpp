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


void server1(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex) {
    int total = 0;

    int dataToSend = generateRandomNumber1(); // Generating a random integer (-90 to 90)
    std::bitset<16> value(dataToSend);


    std::string encodedData = '6' + value.to_string(); // Creating the encoded string

   pthread_mutex_lock(mutex); // Lock the mutex before modifying shared data
   //strcpy(message, encodedData.c_str());
   messageQueue.push(encodedData);
   pthread_cond_signal(condition); // Signal the condition variable
   pthread_mutex_unlock(mutex); // Unlock the mutex

    total += 1;
    //std::cout << "Send: " << encodedData << " num:" << total << std::endl;

    // Introduce some delay to simulate work
    //usleep(1000000); 
    sleep(2);
}
