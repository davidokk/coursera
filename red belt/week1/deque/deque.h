#include <vector>
#include <iostream>
using namespace std;

template<typename T>
class Deque {
 public:
  size_t Size() const {
    return b.size() + e.size();
  }
  bool Empty() const {
    return Size() == 0;
  }

  T& operator[] (size_t index) {
    if (index < b.size())
      return b[b.size() - index - 1];
    return e[index - b.size()];
  }
  const T& operator[] (size_t index) const {
    if (index < b.size())
      return b[b.size() - index - 1];
    return e[index - b.size()];
  }

  T& At(size_t index) {
    if (index < 0 || index >= Size())
      throw out_of_range("");
    return operator[](index);
  }
  const T& At(size_t index) const {
    if (index < 0 || index >= Size())
      throw out_of_range("");
    return operator[](index);
  }

  T& Front() {
    if (b.empty())
      return e.front();
    return b.back();
  }
  const T& Front() const {
    if (b.empty())
      return e.front();
    return b.back();
  }
  T& Back() {
    if (e.empty())
      return b.front();
    return e.back();
  }
  const T& Back() const {
    if (e.empty())
      return b.front();
    return e.back();
  }

  void PushFront(const T& value) {
    b.push_back(value);
  }
  void PushBack(const T& value) {
    e.push_back(value);
  }

 private:
  vector<T> b;
  vector<T> e;
};
