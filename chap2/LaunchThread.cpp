#include <iostream>
#include <thread>

void doSomeWork() {}

class BackgroundTask {
 public:
  void operator()() const {}
};

int main() {
  std::thread myThread(doSomeWork);

  BackgroundTask t;
  std::thread myThread2(t);

  return 0;
}