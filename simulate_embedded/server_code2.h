#ifndef SERVER2_CODE_H
#define SERVER2_CODE_H
#include <string>
#include <queue>

void server2(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex);
int generateRandomNumber2();
int readLines(std::string filePath, std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex);
int sendLineNumber(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex);
int getLineNumber();
void sendServoAngle(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex);
void sendDrawnLine(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex);
#endif
