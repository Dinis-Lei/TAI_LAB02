// add imports to read file
#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char const *argv[]) {
    /* code */

    // read binary file
    std::ifstream file("info_per_byte.txt");
    std::string str;
    while (std::getline(file, str)) {
        std::cout << str << std::endl;
    }

    return 0;
}

