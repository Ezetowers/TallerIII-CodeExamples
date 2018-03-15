// https://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
#include <condition_variable>
#include <mutex>
#include <stdint.h>

class Barrier {
public:
    explicit Barrier(std::size_t count);
    void wait(uint64_t thread_id);

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::size_t count_;
};
