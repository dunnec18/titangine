#include "Core.h"

namespace Core {

    void PrintHelloWorld() {
        std::cout << "Hello World!\n";
        std::cin.get();
    }

    long long parallel_sum(const std::vector<int>& vec, size_t start, size_t end) {
        return std::accumulate(vec.begin() + start, vec.begin() + end, 0LL);
    }

    OrderBook::OrderBook() : lock_free(false) {}

    void OrderBook::add_order(const Order& order) {
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

    void OrderBook::match_orders() {
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
}
