#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>

long long parallel_sum(const std::vector<int>& vec, size_t start, size_t end) {
    return std::accumulate(vec.begin() + start, vec.begin() + end, 0LL);
}

int main() {
    const size_t size = 100000000; // 100 million elements
    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<int> vec(size, 1); // Initialize vector with 1s

    std::vector<std::future<long long>> futures;
    size_t chunk_size = size / num_threads;

    auto start = std::chrono::high_resolution_clock::now();

    // Launch threads to sum parts of the vector
    for (size_t i = 0; i < num_threads; ++i) {
        size_t chunk_start = i * chunk_size;
        size_t chunk_end = (i == num_threads - 1) ? size : chunk_start + chunk_size;
        futures.push_back(std::async(std::launch::async, parallel_sum, std::cref(vec), chunk_start, chunk_end));
    }

    // Collect results from threads
    long long total_sum = 0;
    for (auto& fut : futures) {
        total_sum += fut.get();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Multithreaded sum: " << total_sum << std::endl;
    std::cout << "Time taken (multithreaded): " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
