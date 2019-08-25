#pragma once

#include "msg_queue.h"

namespace messaging {

class sender {
  msg_queue* q_;

 public:
  sender() : q_(nullptr) {}

  explicit sender(msg_queue* q) : q_(q) {}

  template <typename Message>
  void send(Message const& msg) {
    if (q_) {
      q_->push(msg);
    }
  }
};
}  // namespace messaging