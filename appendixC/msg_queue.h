#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace messaging {

struct message_base {
  virtual ~message_base() {}
};

template <typename Msg>
struct wrapped_message : message_base {
  Msg contents_;

  explicit wrapped_message(Msg const& contents) : contents_(contents) {}
};

class msg_queue {
  std::mutex m_;
  std::condition_variable c_;
  std::queue<std::shared_ptr<message_base>> q_;

 public:
  template <typename T>
  void push(T const& msg) {
    std::lock_guard<std::mutex> lk(m_);
    q_.push(std::make_shared<wrapped_message<T>>(msg));
    c_.notify_all();
  }

  std::shared_ptr<message_base> wait_and_pop() {
    std::unique_lock<std::mutex> lk(m_);
    c_.wait(lk, [&] { return !q_.empty(); });
    auto res = q_.front();
    q_.pop();
    return res;
  }
};

}  // namespace messaging