#include <algorithm>
#include <future>
#include <iostream>
#include <list>
#include <random>

template <typename T>
std::list<T> sequentialQuickSort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }

  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  T const& pivot = *result.begin();

  auto dividePoint = std::partition(input.begin(), input.end(),
                                    [&](T const& t) { return t < pivot; });
  std::list<T> lowerPart;
  lowerPart.splice(lowerPart.end(), input, input.begin(), dividePoint);

  auto newLower(sequentialQuickSort(std::move(lowerPart)));
  auto newHigher(sequentialQuickSort(std::move(input)));

  result.splice(result.end(), newHigher);
  result.splice(result.begin(), newLower);

  return result;
}

template <typename T>
std::list<T> parallelQuickSort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }

  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  T const& pivot = *result.begin();

  auto dividePoint = std::partition(input.begin(), input.end(),
                                    [&](T const& t) { return t < pivot; });
  std::list<T> lowerPart;
  lowerPart.splice(lowerPart.end(), input, input.begin(), dividePoint);

  std::future<std::list<T>> newLower(
      std::async(&parallelQuickSort<T>, std::move(lowerPart)));
  auto newHigher(parallelQuickSort(std::move(input)));

  result.splice(result.end(), newHigher);
  result.splice(result.begin(), newLower.get());

  return result;
}

template <typename T>
bool checkList(const std::list<T>& l) {
  T i = 0;
  for (auto& node : l) {
    if (node != i++) {
      return false;
    }
  }
  return true;
}

int main() {
  const int N{10};
  std::vector<int> v(N);
  for (auto i = 0; i < N; i++) {
    v[i] = i;
  }

  std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));

  std::list<int> l;
  std::copy(v.begin(), v.end(), std::back_inserter(l));

  l = sequentialQuickSort(l);
  std::cout << std::boolalpha << checkList(l) << '\n';

  l.clear();
  std::copy(v.begin(), v.end(), std::back_inserter(l));

  l = parallelQuickSort(l);
  std::cout << std::boolalpha << checkList(l) << '\n';

  return 0;
}