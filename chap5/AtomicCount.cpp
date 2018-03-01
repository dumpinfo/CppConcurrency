#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> queue;
std::atomic<int> count;

void populateQueue() {
  const int numberOfItems = 20;
  queue.clear();
  for (auto i = 0; i < numberOfItems; i++) {
    queue.push_back(i);
  }
  count.store(numberOfItems, std::memory_order_release);
}

void consumeQueueItems() {
  while (true) {
    int itemIndex;
    if ((itemIndex = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
      // wait for more items
      continue;
    }

    // process queue[itemIndex - 1]
  }
}

int main() {
  std::thread a(populateQueue);
  std::thread b(consumeQueueItems);
  std::thread c(consumeQueueItems);

  a.join();
  b.join();
  c.join();

  return 0;
}
