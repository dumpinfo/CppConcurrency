#include <future>
#include <string>

struct X {
  void foo(int, std::string const&) {}
  std::string bar(std::string const& x) { return x + "world"; }
};

struct Y {
  double operator()(double) { return 1.0; }
};

class MoveOnly {
 public:
  MoveOnly() = default;
  MoveOnly(MoveOnly&&) = default;
  MoveOnly(MoveOnly const&) = delete;
  MoveOnly& operator=(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly const&) = delete;

  void operator()() {}
};

X baz(X&) { return X{}; }

int main() {
  X x;
  Y y;

  auto f1 = std::async(&X::foo, &x, 42, "hello");
  auto f2 = std::async(&X::bar, x, "goodbye");
  auto f3 = std::async(Y(), 3.141);
  auto f4 = std::async(std::ref(y), 2.718);
  auto f5 = std::async(MoveOnly());
  auto f6 = std::async(std::launch::async, Y(), 1.2);
  auto f7 = std::async(std::launch::deferred, baz, std::ref(x));
  auto f8 =
      std::async(std::launch::deferred | std::launch::async, baz, std::ref(x));
  auto f9 = std::async(baz, std::ref(x));

  return 0;
}