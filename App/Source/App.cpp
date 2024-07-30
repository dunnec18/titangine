#include "/Users/cdunne/git_repos/titangine/Core/Source/Core.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>

int main() {
    Core::OrderBook order_book;

    // Generate and add orders
    for (int i = 0; i < 1000000; ++i) {
        Core::Order order = {i, rand() % 100, rand() % 10 + 1, rand() % 2 == 0};
        order_book.add_order(order);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Match orders
    order_book.match_orders();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken for order matching: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
