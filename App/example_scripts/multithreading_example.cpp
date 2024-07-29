#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// Mutex for synchronizing output
std::mutex printMutex;

// Function that each thread will run
void threadFunction(int threadNumber) {
    std::lock_guard<std::mutex> guard(printMutex);
    std::cout << "Hello from thread " << threadNumber << std::endl;
}

int main() {
    // Vector to hold thread objects
    std::vector<std::thread> threads;

    // Create and start 5 threads
    for (int i = 1; i <= 5; ++i) {
        threads.push_back(std::thread(threadFunction, i));
    }

    // Wait for all threads to complete
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "All threads have completed." << std::endl;

    return 0;
}
