#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>
#include <atomic>

namespace Core {

    void PrintHelloWorld();

    struct Order {
        int id;
        int price;
        int quantity;
        bool is_buy; // true for buy, false for sell

        bool operator<(const Order& other) const {
            return price < other.price;
        }

        bool operator>(const Order& other) const {
            return price > other.price;
        }
    };

    long long parallel_sum(const std::vector<int>& vec, size_t start, size_t end);

    class OrderBook {
    private:
        std::vector<Order> buy_orders;
        std::vector<Order> sell_orders;
        std::atomic<bool> lock_free;

    public:
        OrderBook();

        void add_order(const Order& order);

        void match_orders();
    };
}
