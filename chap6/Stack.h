#include <exception>
#include <mutex>
#include <stack>
#include <memory>

struct empty_stack : std::exception {
  const char* what() const noexcept override { return exception::what(); }
};

template <typename T>
class ThreadSafeStack {
 public:
  ThreadSafeStack() = default;

  ThreadSafeStack(const ThreadSafeStack& other) {
    std::lock_guard<std::mutex> lock(other.m_);
    data_ = other.data_;
  }

  ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

  void push(T new_value) {
    std::lock_guard<std::mutex> lock(m_);
    data_.push(std::move(new_value));
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(m_);
    if (data_.empty()) {
      throw empty_stack();
    }

    const auto res = std::make_shared<T>(std::move(data_.top()));
    data_.pop();

    return res;
  }

  void pop(T& value) {
    std::lock_guard<std::mutex> lock(m_);
    if (data_.empty()) {
      throw empty_stack();
    }

    value = std::move(data_.top());
    data_.pop();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(m_);
    return data_.empty();
  }

 private:
  std::stack<T> data_;
  mutable std::mutex m_;
};
