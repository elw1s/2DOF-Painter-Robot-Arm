#ifndef SERVER1_CODE_H
#define SERVER1_CODE_H
#include <queue>
#include <string>

void server1(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex);
int generateRandomNumber1();
#endif