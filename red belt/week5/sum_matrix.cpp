#include "test_runner.h"

#include <iostream>
#include <future>
#include <vector>

using namespace std;

template <typename Iterator>
class IteratorRange {
 public:
  IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

  Iterator begin() const { return begin_; }
  Iterator end() const { return end_; }

 private:
  Iterator begin_;
  Iterator end_;
};

template <typename Iterator>
auto MakeIteratorRange(Iterator begin, Iterator end) {
  return IteratorRange<Iterator>(begin, end);
}

template <typename Iterator>
using Page = IteratorRange<Iterator>;

template <typename Iterator>
class Paginator {
 public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    while (begin != end) {
      Iterator page_end;
      if (end - begin >= page_size) {
        page_end = begin + page_size;
      } else {
        page_end = end;
      }
      pages.push_back(MakeIteratorRange(begin, page_end));
      begin = page_end;
    }
  }

  auto begin() { return pages.begin(); }
  auto end() { return pages.end(); }

 private:
  std::vector<Page<Iterator>> pages;
};

template <typename Iterator>
auto MakePaginator(Iterator begin, Iterator end, size_t page_size) {
  return Paginator<Iterator>(begin, end, page_size);
}

template <typename Container>
int64_t SumSingle(const Container& matrix) {
  int64_t sum = 0;
  for (const auto& row : matrix) {
    for (auto i : row)
      sum += i;
  }
  return sum;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  vector<future<int64_t>> futures;
  for (auto page : MakePaginator(matrix.begin(), matrix.end(), 2000)) {
    futures.push_back(async([=]{
      return SumSingle(page);
    }));
  }

  int64_t sum = 0;
  for (auto& f : futures)
    sum += f.get();

  return sum;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}