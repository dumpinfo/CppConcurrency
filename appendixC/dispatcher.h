#pragma once

#include "msg_queue.h"

namespace messaging {

class close_queue {};

template <typename PreviousDispatcher, typename Msg, typename Func>
class TemplateDispatcher {
  msg_queue* q_;
  PreviousDispatcher* prev_;
  Func f_;
  bool chained_;

  TemplateDispatcher(TemplateDispatcher const&) = delete;
  TemplateDispatcher& operator=(TemplateDispatcher const&) = delete;

  template <typename Dispatcher, typename OtherMsg, typename OtherFunc>
  friend class TemplateDispatcher;

  void wait_and_dispatch() {
    for (;;) {
      auto msg = q_->wait_and_pop();
      if (dispatch(msg)) {
        break;
      }
    }
  }

  bool dispatch(std::shared_ptr<message_base> const& msg) {
    if (wrapped_message<Msg>* wrapper =
            dynamic_cast<wrapped_message<Msg>*>(msg.get())) {
      f_(wrapper->contents_);
      return true;
    } else {
      return prev_->dispatch(msg);
    }
  }

 public:
  TemplateDispatcher(TemplateDispatcher&& other)
      : q_(other.q_),
        prev_(other.prev_),
        f_(std::move(other.f)),
        chained_(other.chained_) {
    other.chained_ = true;
  }

  TemplateDispatcher(msg_queue* q, PreviousDispatcher* prev, Func&& f)
      : q_(q), prev_(prev), f_(std::forward<Func>(f)), chained_(false) {
    prev->chained_ = true;
  }

  template <typename OtherMsg, typename OtherFunc>
  TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc> handle(
      OtherFunc&& of) {
    return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(
        q_, this, std::forward<OtherFunc>(of));
  }

  ~TemplateDispatcher() noexcept(false) {
    if (!chained_) {
      wait_and_dispatch();
    }
  }
};

class dispatcher {
  msg_queue* q_;
  bool chained_;

  template <typename Dispatcher, typename Msg, typename Func>
  friend class TemplateDispatcher;

  void wait_and_dispatch() {
    for (;;) {
      auto msg = q_->wait_and_pop();
      dispatch(msg);
    }
  }

  static bool dispatch(std::shared_ptr<message_base> const& msg) {
    if (dynamic_cast<wrapped_message<close_queue>*>(msg.get())) {
      throw close_queue();
    }
    return false;
  }

 public:
  dispatcher(dispatcher const&) = delete;
  dispatcher& operator=(dispatcher const&) = delete;

 public:
  dispatcher(dispatcher&& other) noexcept
      : q_(other.q_), chained_(other.chained_) {
    other.chained_ = true;
  }

  explicit dispatcher(msg_queue* q) : q_(q), chained_(false) {}

  template <typename Message, typename Func>
  TemplateDispatcher<dispatcher, Message, Func> handle(Func&& f) {
    return TemplateDispatcher<dispatcher, Message, Func>(q_, this,
                                                         std::forward<Func>(f));
  }

  ~dispatcher() noexcept(false) {
    if (!chained_) {
      wait_and_dispatch();
    }
  }
};
}  // namespace messaging