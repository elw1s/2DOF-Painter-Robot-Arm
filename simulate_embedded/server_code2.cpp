#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <bitset>
#include "server_code2.h"
#include <random>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <algorithm>


int numberOfLines = 0;   // Global variable to store the number of lines
std::vector<std::string> lines;
std::vector<std::string> temp;

int generateRandomNumber2() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-180, 180);
    return dis(gen);
}

int readLines(std::string filePath, std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex){
    
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        return -1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string jsonData;
    char ch;
    int bracketCounter = 0;
    std::streamsize bytesRead = 0;

    while (file.get(ch) && bracketCounter < 3) {
        jsonData += ch;
        bytesRead++;

        if (ch == '[') {
            bracketCounter--;
        } else if (ch == ']') {
            bracketCounter++;
        }
    }

    file.close();

        std::cout << "\n\n\n" << jsonData << "\n\n\n";

    rapidjson::Document doc;
    doc.Parse(jsonData.c_str());

    size_t chunkSize;
    int offset = 0;
    while (offset < size) {
        chunkSize = std::min<size_t>(4096, static_cast<size_t>(size - offset));

        char* messageBuffer = new char[chunkSize + 2]; // +1 for '9' character, +1 for null-termination
        messageBuffer[0] = '2'; // Set the first character to '9'

        // Copy the data into the message buffer
        strncpy(messageBuffer + 1, jsonData.c_str() + offset, chunkSize);
        messageBuffer[chunkSize + 1] = '\0'; // Null-terminate the string

        pthread_mutex_lock(mutex);
        messageQueue.push(messageBuffer);
        printf("%ldB put into messageQueue\n", chunkSize);

        // Signal that a new message is available
        pthread_cond_signal(condition);
        printf("Signaled\n");
        pthread_mutex_unlock(mutex);

        offset += chunkSize;
    }
    if (doc.HasParseError()) {
            std::cerr << "JSON parse error\n";
            delete[] jsonData.c_str();
            return -1;
    }

    // Check if it's an array of arrays (2D array)
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
        if (doc[i].IsArray()) {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc[i].Accept(writer);
            lines.push_back(buffer.GetString());
        }
    }
    
    std::cout << "Number of lines: "<< numberOfLines << std::endl;
    numberOfLines = lines.size();
    temp = lines;
    return numberOfLines;
}

int sendLineNumber(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex){
    std::string dataToSend = '3' + std::to_string(numberOfLines);
    pthread_mutex_lock(mutex);
    //strcpy(message, dataToSend.c_str());
    messageQueue.push(dataToSend);
    pthread_cond_signal(condition);
    pthread_mutex_unlock(mutex);
    usleep(500000);
    return numberOfLines;
}

void sendDrawnLine(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    if(!lines.empty()){
            std::string line = lines.at(0);
            line = '4' + line;
            //strcpy(message, line.c_str());
            messageQueue.push(line);
            lines.erase(lines.begin());    
        }
    pthread_cond_signal(condition); // Signal the condition variable
    pthread_mutex_unlock(mutex);
    usleep(500000); // 0.5 seconds
}

void sendServoAngle(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex){
    std::vector<int> angles;
    for (int i = 0; i < 3; ++i) {
        int angle = generateRandomNumber2(); // Generate angles between -180 and +180 degrees
        angles.push_back(angle);
    }

    std::bitset<16> angle1(angles[0]);
    std::bitset<16> angle2(angles[1]);
    std::bitset<16> angle3(angles[2]);

    std::string dataToSend = '5' + angle1.to_string() + angle2.to_string() + angle3.to_string(); // DECODER + 3 angles
    pthread_mutex_lock(mutex);
    //strcpy(message, dataToSend.c_str());
    messageQueue.push(dataToSend);
    pthread_cond_signal(condition); // Signal the condition variable
    pthread_mutex_unlock(mutex);
    usleep(500000); // 0.5 seconds
}


int getLineNumber(){
    return numberOfLines;
}
 

bool server2(std::queue<std::string>& messageQueue, pthread_cond_t* condition, pthread_mutex_t* mutex) {
    if(lines.size() == 0){
        pthread_mutex_lock(mutex);
        messageQueue.push("0Connection Established.");
        pthread_cond_signal(condition); // Signal the condition variable
        pthread_mutex_unlock(mutex);
        return false;
    } 
   sendDrawnLine(messageQueue,condition,mutex);
   sendServoAngle(messageQueue,condition,mutex);
   return true;
}
