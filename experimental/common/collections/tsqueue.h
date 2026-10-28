#pragma once
#include <optional>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace common  {
    template <typename T>
    class tsqueue {
        public:
            void push(const T & value) {
                std::lock_guard<std::mutex> lock(mutex);
                queue.push(value);
                condition.notify_one();
            }

            std::optional<T> pop() {
                std::unique_lock<std::mutex> lock(mutex);
                if (queue.empty()) return std::nullopt;
                T value = queue.front();
                queue.pop();
                return value;
            }

            bool empty() const {
                std::lock_guard<std::mutex> lock(mutex);
                return queue.empty();
            }

            int size() const {
                std::lock_guard<std::mutex> lock(mutex);
                return queue.size();
            }

        private:
            std::queue<T> queue;
            mutable std::mutex mutex;
            std::condition_variable condition;
    };
}

template<typename T>
using ThreadSafeQueue = common::tsqueue<T>;
