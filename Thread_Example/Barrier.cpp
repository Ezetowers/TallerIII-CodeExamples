// https://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
#include "Barrier.hpp"
#include <iostream>


Barrier::Barrier(std::size_t count) : count_(count) {}

void Barrier::wait(uint64_t thread_id) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    if (--this->count_ == 0) {
        std::cout << "[THREAD N°" << thread_id 
                  << "] Proceed to wake up all threads. "
                  << "Has reached the barrier." << std::endl;
        cv_.notify_all();
    } 
    else {
        std::cout << "[THREAD N°" << thread_id 
                  << "] Waiting in Barrier" << std::endl; 
        cv_.wait(lock, [this] { return count_ == 0; });
        std::cout << "[THREAD N°" << thread_id 
                  << "] Thread has been woken up." << std::endl;
    }
}
