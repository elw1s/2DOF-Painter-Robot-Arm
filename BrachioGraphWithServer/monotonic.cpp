#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    std::string filename = "/tmp/cse396/sent.json";
    std::ifstream file(filename, std::ios::binary); // Open file in binary mode

    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::streampos fileSize = buffer.tellg(); // Get the number of bytes read

        if (fileSize != -1) {
            std::cout << "BG Number of bytes read: " << fileSize << std::endl;
        } else {
            std::cerr << "Error determining file size!" << std::endl;
        }
    } else {
        std::cerr << "Unable to open file!" << std::endl;
    }

    std::ifstream file2(filename, std::ios::binary | std::ios::ate);
    if (!file2.is_open()) {
        std::cerr << "Unable to open file2\n";
        return -1;
    }

    std::streamsize size = file2.tellg();
    file2.seekg(0, std::ios::beg);

    std::string jsonData;
    char ch;
    int bracketCounter = 0;
    std::streamsize bytesRead = 0;

    while (file2.get(ch) && bracketCounter < 3) {
        jsonData += ch;
        bytesRead++;

        if (ch == '[') {
            bracketCounter--;
        } else if (ch == ']') {
            bracketCounter++;
        }
    }
    std::cout << "MS Number of bytes read: " << bytesRead << std::endl;
    file2.close();




    return 0;
}
