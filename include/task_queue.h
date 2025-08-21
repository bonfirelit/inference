#pragma once

#include "common.h"

class Tensor;

// -----------------------------
// Task 定义
// -----------------------------
struct Task {
    std::vector<Tensor> inputs;                        // 输入张量
    std::function<void(std::vector<Tensor>&&)> cb;      // 回调函数（输出）

    Task() = default;
    ~Task() = default;

    Task(std::vector<Tensor> in,
         std::function<void(std::vector<Tensor>&&)> callback)
        : inputs(std::move(in)), cb(std::move(callback)) {}
};

// -----------------------------
// TaskQueue 定义
// -----------------------------
class TaskQueue {
public:
    TaskQueue() = default;
    ~TaskQueue() = default;
    
    void push(Task task) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(task));
        }
        cond_.notify_one(); // 唤醒一个等待中的线程
    }

    bool pop(Task& task_out) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return stop_ || !queue_.empty(); });

        if (stop_ && queue_.empty()) {
            if (stop_) {
                INFO_LOG("Task Queue stopped! and the queue's size = %ld", queue_.size());
            }
            return false;
        }

        task_out = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cond_.notify_all(); // 唤醒所有等待线程
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<Task> queue_;
    bool stop_;
};
