#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>

template <typename Iterator, typename T>
struct accumulateBlock {
  void operator()(Iterator first, Iterator last, T& result) {
    result = std::accumulate(first, last, result);
  }
};

template <typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init) {
  auto length = std::distance(first, last);
  if (!length) {
    return init;
  }

  auto minPerThread = 25;
  auto maxThreads = (length + minPerThread - 1) / minPerThread;

  auto hardwareThreads = std::thread::hardware_concurrency();
  long x = hardwareThreads != 0 ? hardwareThreads : 2;
  auto numThreads = (size_t)std::min(x, maxThreads);
  auto blockSize = length / numThreads;

  std::vector<T> results(numThreads);
  std::vector<std::thread> threads(numThreads - 1);

  auto blockStart = first;
  for (auto i = 0; i < numThreads - 1; i++) {
    auto blockEnd = blockStart;
    std::advance(blockEnd, blockSize);
    threads[i] = std::thread(accumulateBlock<Iterator, T>(), blockStart,
                             blockEnd, std::ref(results[i]));
    blockStart = blockEnd;
  }
  accumulateBlock<Iterator, T>()(blockStart, last, results[numThreads - 1]);

  std::for_each(threads.begin(), threads.end(),
                std::mem_fn(&std::thread::join));

  return std::accumulate(results.begin(), results.end(), init);
};

int main() {
  const int N{100};
  std::vector<int> v(N);
  for (auto i = 0; i < N; i++) {
    v[i] = i;
  }

  std::cout << parallelAccumulate(v.begin(), v.end(), 0) << std::endl;

  return 0;
}