#ifndef SERVER2_CODE_H
#define SERVER2_CODE_H
#include <string>

void server2(char* message, pthread_cond_t* condition, pthread_mutex_t* mutex);
int generateRandomNumber2();
int readLines(std::string filePath);
int sendLineNumber(char* message, pthread_cond_t* condition, pthread_mutex_t* mutex);
#endif
