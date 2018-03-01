#pragma once

#include <thread>
#include <vector>

class JoinThreads {
 public:
  explicit JoinThreads(std::vector<std::thread>& threads) : threads_(threads) {}

  ~JoinThreads() {
    for (auto& t : threads_) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

 private:
  std::vector<std::thread>& threads_;
};