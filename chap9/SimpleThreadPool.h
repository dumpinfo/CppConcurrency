#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include "../chap6/Queue.h"
#include "../chap8/JoinThreads.h"

class SimpleThreadPool {
 public:
  SimpleThreadPool() : joiner_(threads_), done_(false) {
    auto threadCount = std::thread::hardware_concurrency();

    try {
      for (auto i = 0; i < threadCount; i++) {
        threads_.emplace_back(&SimpleThreadPool::workerThread, this);
      }
    } catch (...) {
      done_ = true;
      throw;
    }
  }

  ~SimpleThreadPool() { done_ = true; }

  template <typename FunctionType>
  void submit(FunctionType f) {
    work_queue_.push(std::function<void()>(f));
  }

 private:
  void workerThread() {
    while (!done_) {
      std::function<void()> task;
      if (work_queue_.tryPop(task)) {
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }

 private:
  JoinThreads joiner_;
  std::atomic_bool done_;
  ThreadSafeQueue<std::function<void()>> work_queue_;
  std::vector<std::thread> threads_;
};
