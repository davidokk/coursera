#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  if (range_end - range_begin < 2)
    return;
  vector<typename RandomIt::value_type>
      v(make_move_iterator(range_begin), make_move_iterator(range_end));
  auto left = v.begin() + (range_end - range_begin) / 3;
  auto right = v.begin() + 2 * (range_end - range_begin) / 3;

  MergeSort(v.begin(), left);
  MergeSort(left, right);
  MergeSort(right, v.end());

  vector<typename RandomIt::value_type> t;
  merge(make_move_iterator(v.begin()),
        make_move_iterator(left),
        make_move_iterator(left),
        make_move_iterator(right),
        back_inserter(t));
  merge(make_move_iterator(t.begin()),
        make_move_iterator(t.end()),
        make_move_iterator(right),
        make_move_iterator(v.end()),
        range_begin);
}


  void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
  }

  int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
  }