#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


int readLines(std::string filePath){
    int numberOfLines = 0;
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

        size_t chunkSize;
        int offset = 0;
        while (offset < size) {
            chunkSize = std::min<size_t>(4096, static_cast<size_t>(size - offset));

            char* messageBuffer = new char[chunkSize + 2]; // +1 for '9' character, +1 for null-termination
            messageBuffer[0] = '2'; // Set the first character to '9'

            // Copy the data into the message buffer
            strncpy(messageBuffer + 1, jsonData + offset, chunkSize);
            messageBuffer[chunkSize + 1] = '\0'; // Null-terminate the string

            printf("%ldB put into messageQueue\n", chunkSize);

            offset += chunkSize;
        }
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
                numberOfLines++;
            }
        }

        return numberOfLines;
        
}

int main(){
    int num = readLines("tmp/sent.json");
    std::cout << "Number:" << num << std::endl;
}
