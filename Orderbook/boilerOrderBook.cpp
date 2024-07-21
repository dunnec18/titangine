#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>

// Define a structure for orders
struct Order {
    int id;
    int price;
    int quantity;
    bool is_buy; // true for buy, false for sell

    // Define comparison operator for heap operations
    bool operator<(const Order& other) const {
        return price < other.price;
    }

    // Define greater-than operator for heap operations
    bool operator>(const Order& other) const {
        return price > other.price;
    }
};

long long parallel_sum(const std::vector<int>& vec, size_t start, size_t end) {
    return std::accumulate(vec.begin() + start, vec.begin() + end, 0LL);
}

class OrderBook {
private:
    std::vector<Order> buy_orders;
    std::vector<Order> sell_orders;
    std::atomic<bool> lock_free;

public:
    OrderBook() : lock_free(false) {}

    void add_order(const Order& order) {
        while (lock_free.exchange(true)) {} // Spin-lock
        if (order.is_buy) {
            buy_orders.push_back(order);
            std::push_heap(buy_orders.begin(), buy_orders.end());
        } else {
            sell_orders.push_back(order);
            std::push_heap(sell_orders.begin(), sell_orders.end(), std::greater<Order>());
        }
        lock_free = false;
    }

    void match_orders() {
        while (!buy_orders.empty() && !sell_orders.empty()) {
            if (buy_orders.front().price >= sell_orders.front().price) {
                // Match orders (for simplicity, just print matched orders)
                std::cout << "Matched Buy Order ID: " << buy_orders.front().id
                          << " with Sell Order ID: " << sell_orders.front().id << std::endl;

                // Remove matched orders
                std::pop_heap(buy_orders.begin(), buy_orders.end());
                buy_orders.pop_back();
                std::pop_heap(sell_orders.begin(), sell_orders.end(), std::greater<Order>());
                sell_orders.pop_back();
            } else {
                break;
            }
        }
    }
};

int main() {
    OrderBook order_book;

    // Generate and add orders
    for (int i = 0; i < 1000000; ++i) {
        Order order = {i, rand() % 100, rand() % 10 + 1, rand() % 2 == 0};
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
