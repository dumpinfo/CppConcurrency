#pragma once

#include <deque>
#include <mutex>

#include "FunctionWrapper.h"

class WorkStealingQueue {
  using DataType = FunctionWrapper;

 public:
  WorkStealingQueue() = default;

  WorkStealingQueue(const WorkStealingQueue& other) = delete;
  WorkStealingQueue& operator=(const WorkStealingQueue& other) = delete;

  void push(DataType data) {
    std::lock_guard<std::mutex> lock(mu_);
    queue_.push_front(std::move(data));
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(mu_);
    return queue_.empty();
  }

  bool tryPop(DataType& res) {
    std::lock_guard<std::mutex> lock(mu_);
    if (queue_.empty()) {
      return false;
    }

    res = std::move(queue_.front());
    queue_.pop_front();
    return true;
  }

  bool trySteal(DataType& res) {
    std::lock_guard<std::mutex> lock(mu_);
    if (queue_.empty()) {
      return false;
    }

    res = std::move(queue_.back());
    queue_.pop_back();
    return true;
  }

 private:
  std::deque<DataType> queue_;
  mutable std::mutex mu_;
};