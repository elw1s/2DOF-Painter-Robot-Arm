#include <iostream>
#include "brachiograph.cpp"
#include <queue>



std::queue<std::string> messagesWaitingToBeSend;
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataCond = PTHREAD_COND_INITIALIZER;



int main(){
    BrachioGraph bg = BrachioGraph(messagesWaitingToBeSend, &dataCond, &dataMutex);
    bg.plot_file("/home/ardakilic/Desktop/CSE396/simulate_embedded/tmp/received.json");
    //bg.pen->down();
    //bg.set_angles(0,90);
    //bg.park();
}