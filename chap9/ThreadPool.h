#pragma once

#include "FunctionWrapper.h"

#include <functional>
#include <future>
#include <thread>
#include <vector>
#include "../chap6/Queue.h"
#include "../chap8/JoinThreads.h"

class ThreadPool {
 public:
  ThreadPool() : joiner_(threads_), done_(false) {
    auto threadCount = std::thread::hardware_concurrency();

    try {
      for (auto i = 0; i < threadCount; i++) {
        threads_.emplace_back(&ThreadPool::workerThread, this);
      }
    } catch (...) {
      done_ = true;
      throw;
    }
  }

  ~ThreadPool() { done_ = true; }

  template <typename FunctionType>
  std::future<std::result_of_t<FunctionType()>> submit(FunctionType f) {
    using result_type = std::result_of_t<FunctionType()>;

    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    work_queue_.push(std::move(task));
    return res;
  }

 private:
  void workerThread() {
    while (!done_) {
      FunctionWrapper task;
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
  ThreadSafeQueue<FunctionWrapper> work_queue_;
  std::vector<std::thread> threads_;
};
