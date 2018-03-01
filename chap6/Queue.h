#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue {
 public:
  ThreadSafeQueue() = default;

  void push(T newValue) {
    std::lock_guard<std::mutex> lk(mut_);
    data_.push(std::move(data_));
    cond_.notify_one();
  }

  void waitAndPop(T& value) {
    std::unique_lock<std::mutex> lk(mut_);
    cond_.wait(lk, [this] { return !data_.empty(); });
    value = std::move(data_.front());
    data_.pop();
  }

  std::shared_ptr<T> waitAndPop() {
    std::unique_lock<std::mutex> lk(mut_);
    cond_.wait(lk, [this] { return !data_.empty(); });
    auto res = std::make_shared<T>(std::move(data_.front()));
    data_.pop();
    return res;
  }

  bool tryPop(T& value) {
    std::lock_guard<std::mutex> lk(mut_);
    if (data_.empty()) {
      return false;
    }

    value = std::move(data_.front());
    data_.pop();

    return true;
  }

  std::shared_ptr<T> tryPop() {
    std::lock_guard<std::mutex> lk(mut_);
    if (data_.empty()) {
      return std::shared_ptr<T>();
    }

    auto res = std::make_shared<T>(std::move(data_.front()));
    data_.pop();

    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(mut_);
    return data_.empty();
  }

 private:
  mutable std::mutex mut_;
  std::queue<T> data_;
  std::condition_variable cond_;
};
