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

int generateRandomNumber2() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-180, 180);
    return dis(gen);
}

int readLines(std::string filePath){
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cerr << "Unable to open file\n";
                return -1;
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            char* jsonData = new char[size];
            if (!file.read(jsonData, size)) {
                std::cerr << "Failed to read file\n";
                file.close();
                delete[] jsonData;
                return -1;
            }
            file.close();

            rapidjson::Document doc;
            doc.Parse(jsonData);

            if (doc.HasParseError()) {
                    std::cerr << "JSON parse error\n";
                    delete[] jsonData;
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

            numberOfLines = lines.size();
}

int sendLineNumber(char* message, pthread_cond_t* condition, pthread_mutex_t* mutex){
    std::string dataToSend = '0' + std::to_string(numberOfLines);
    pthread_mutex_lock(mutex);
    strcpy(message, dataToSend.c_str());
    pthread_cond_signal(condition);
    pthread_mutex_unlock(mutex);
    usleep(2000000);
}

void server2(char* message, pthread_cond_t* condition, pthread_mutex_t* mutex) {
    int total = 0;
    std::cout << "Server 2 function started for client\n";

    std::vector<int> angles;
    for (int i = 0; i < 3; ++i) {
        int angle = generateRandomNumber2(); // Generate angles between -180 and +180 degrees
        angles.push_back(angle);
    }

    std::bitset<16> angle1(angles[0]);
    std::bitset<16> angle2(angles[1]);
    std::bitset<16> angle3(angles[2]);

    std::cout << "Created Angle 1:" << angle1.to_string() << std::endl;
    std::cout << "Created Angle 2:" << angle2.to_string() << std::endl;
    std::cout << "Created Angle 3:" << angle3.to_string() << std::endl;

    std::string dataToSend = '3' + angle1.to_string() + angle2.to_string() + angle3.to_string(); // DECODER + 3 angles

    printf("SERVER2: Locking mutex...\n");
    pthread_mutex_lock(mutex); // Lock the mutex before modifying shared data
    printf("SERVER2: Locked mutex...\n");

    // Burada öylesine rastgele dosya yollanmıştır
    if(generateRandomNumber2() > 0){
        if(!lines.empty()){
            std::string line = lines.at(0);
            line = '1' + line;
            strcpy(message, line.c_str());
            lines.erase(lines.begin());    
        }
    }
    else{
        strcpy(message, dataToSend.c_str());
    }

    pthread_cond_signal(condition); // Signal the condition variable
    printf("SERVER2: Sent signal...\n");
    pthread_mutex_unlock(mutex); // Unlock the mutex
    printf("SERVER2: Unlocked mutex...\n");
    
    total += 1;
    std::cout << "Send: " << dataToSend << " num:" << total << std::endl;

    usleep(2000000); // Sleep for 2 seconds (2 million microseconds)
}
