#ifndef SERVER1_CODE_H
#define SERVER1_CODE_H

void server1(char* message, pthread_cond_t* condition, pthread_mutex_t* mutex);
int generateRandomNumber1();
#endif