#include <iostream>
#include <numeric>
#include "ThreadPool.h"

template <typename Iterator, typename T>
struct accumulateBlock {
  T operator()(Iterator first, Iterator last) {
    return std::accumulate(first, last, 0);
  }
};

template <typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init) {
  auto length = std::distance(first, last);
  if (!length) {
    return init;
  }

  auto blockSize = 25;
  auto numBlocks = (length + blockSize - 1) / blockSize;

  std::vector<std::future<T>> futures(
      static_cast<unsigned long>(numBlocks - 1));
  ThreadPool pool;

  Iterator blockStart = first;
  for (auto i = 0; i < numBlocks - 1; i++) {
    Iterator blockEnd = blockStart;
    std::advance(blockEnd, blockSize);
    futures[i] =
        pool.submit(accumulateBlock<Iterator, T>()(blockStart, blockEnd));
    blockStart = blockEnd;
  }

  T lastResult = accumulateBlock<Iterator, T>()(blockStart, last);
  T result = init;
  for (auto i = 0; i < numBlocks - 1; i++) {
    result += futures[i].get();
  }
  result += lastResult;
  return result;
}

int main() {
  const int N{100};
  std::vector<int> v(N);
  for (auto i = 0; i < N; i++) {
    v[i] = i;
  }

  // std::cout << parallelAccumulate(v.begin(), v.end(), 0) << std::endl;

  return 0;
}