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
#include "Barrier.hpp"
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
         Barrier & barrier) {

    // Wait for all the threads to be created before starting
    barrier.wait(id);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (uint32_t i = 0; i < loop_iterations; ++i) {
        std::string md5_digest(md5(buffer.str()));
    }


    // std::cout << "Thread N°" << id 
    //           << " - Time Elapsed: " << time_elapsed 
    //           << " - Thread Cont: " << thread_cont[id] 
    //           << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    thread_cont[id] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}


int main(int argc, char* argv[]) {
    std::vector<std::thread> workers;
  
    if (argc != 4) {
        std::cout << "Usage: " << std::endl;
        std::cout << "\t./test [AMOUNT_THREADS] [LOOP_ITERATIONS] [FILE_TO_PROCESS]" << std::endl;
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
    Barrier barrier(amount_threads);

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (uint64_t i = 0; i < amount_threads; ++i) {
        workers.push_back(std::thread(svc, 
                                      i,
                                      loop_iterations,
                                      std::ref(buffer),
                                      std::ref(thread_cont), 
                                      std::ref(barrier)));
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    uint64_t thread_creation_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

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
    std::cout << "Avg. Loop time per thread: " 
              << (float) total_avg_time / amount_threads
              << " us." << std::endl;
    return 0;
}
