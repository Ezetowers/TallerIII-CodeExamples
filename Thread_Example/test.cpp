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

typedef std::map<uint64_t, uint64_t> MD5_Map;
typedef MD5_Map::iterator MD5_Map_It;
typedef MD5_Map::const_iterator MD5_Map_Const_It;

typedef std::vector<uint64_t> Thread_Cont;
typedef Thread_Cont::iterator Thread_Cont_It;
typedef Thread_Cont::const_iterator Thread_Cont_Const_It;

void read_file(std::stringstream & buffer, const std::string & path) {
    std::ifstream t(path);
    buffer << t.rdbuf();
    t.close();
}


void svc(uint64_t id,
         uint32_t loop_iterations,
         const std::stringstream & buffer,
         Thread_Cont & thread_cont,
         std::mutex & m) {

    for (uint32_t i = 0; i < loop_iterations; ++i) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // Avg. time: ~40us
        std::string md5_digest(md5(buffer.str()));

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        uint64_t time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        thread_cont[id] += time_elapsed;
    }
}


int main(int argc, char* argv[]) {
    std::vector<std::thread> workers;
    std::mutex m;
  
    if (argc != 4) {
        std::cout << "Two argument must be given to the program. Aborting." << std::endl;
        exit(1);
    }

    // Create a array with as many elements as threads will be created
    const uint64_t amount_threads = std::stoi(argv[1]);
    const uint32_t loop_iterations = std::stoi(argv[2]);
    Thread_Cont thread_cont;
    thread_cont.reserve(amount_threads);

    // Read the file to digest
    std::stringstream buffer;
    read_file(buffer, argv[3]);

    // Create N threads and measure the time it takes to create every of them
    uint64_t thread_creation_time = 0;
    for (uint64_t i = 0; i < amount_threads; ++i) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        workers.push_back(std::thread(svc, 
                                      i,
                                      loop_iterations,
                                      std::ref(buffer),
                                      std::ref(thread_cont), 
                                      std::ref(m)));
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        thread_creation_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    // Wait for the threads to finish
    for (auto& worker : workers) {
        worker.join();
    }

    std::cout << "Avg. Thread Creation time: " 
              << (float) thread_creation_time / amount_threads
              << " us." << std::endl;

    float total_avg_time = 0;
    for (uint32_t i = 0; i < amount_threads; ++i) {
        total_avg_time += (float) thread_cont[i] / loop_iterations;
    }

    // Compute the avg. time of all threads 
    std::cout << "Avg. Context Switch time per thread: " 
              << (float) total_avg_time / amount_threads
              << " us." << std::endl;
    return 0;
}
