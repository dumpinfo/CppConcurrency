#pragma once

class FunctionWrapper {
 public:
  template <typename F>
  explicit FunctionWrapper(F&& f) : impl_(new ImplType<F>(std::move(f))) {}

  void operator()() { impl_->call(); }

  FunctionWrapper() = default;

  FunctionWrapper(FunctionWrapper&& other) noexcept
      : impl_(std::move(other.impl_)) {}

  FunctionWrapper& operator=(FunctionWrapper&& other) noexcept {
    impl_ = std::move(other.impl_);
    return *this;
  }

  FunctionWrapper(const FunctionWrapper&) = delete;
  FunctionWrapper(FunctionWrapper&) = delete;
  FunctionWrapper& operator=(const FunctionWrapper&) = delete;

 private:
  struct ImplBase {
    virtual void call() = 0;
    virtual ~ImplBase() = default;
  };

  template <typename F>
  struct ImplType : ImplBase {
    F f_;
    explicit ImplType(F&& f) : f_(std::move(f)) {}
    void call() override { f_(); }
  };

  std::unique_ptr<ImplBase> impl_;
};
