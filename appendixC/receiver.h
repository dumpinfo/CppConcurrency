#pragma once

#include "dispatcher.h"
#include "msg_queue.h"
#include "sender.h"

namespace messaging {

class receiver {
  msg_queue q_;

 public:
  explicit operator sender() { return sender(&q_); }

  dispatcher wait() { return dispatcher(&q_); }
};
}  // namespace messaging