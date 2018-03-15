// C includes
#include <pthread.h>
#include <stdint.h>

// C++ includes
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
#include <utility>

// Project includes
#include "md5.h"


void read_file(std::stringstream & buffer, const std::string & path) {
    std::ifstream t(path);
    buffer << t.rdbuf();
}


int main(int argc, char* argv[]) {  
    if (argc != 2) {
        std::cout << "The file to hash has not been received. Aborting." << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    read_file(buffer, argv[1]);

    uint64_t time_elapsed = 0;
    for (uint32_t i = 0; i < 100; ++i) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        md5(buffer.str().c_str());
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        time_elapsed += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }


    std::cout << "MD5 compute time: " 
              << time_elapsed / 100
              << " us."
              << std::endl;
    return 0;
}
