#pragma once

#include <stdexcept>
#include <array>
using namespace std;

template <typename T, size_t N>
class StackVector {
 public:
  explicit StackVector(size_t a_size = 0) {
    if (a_size > Capacity())
      throw invalid_argument("");
    size_ = a_size;
  }

  T& operator[](size_t index) { return data_[index]; }
  const T& operator[](size_t index) const { return data_[index]; }

  auto begin() { return data_.begin(); }
  auto end() { return data_.begin() + size_; }
  auto begin() const { return data_.begin(); }
  auto end() const { return data_.begin() + size_; }

  size_t Size() const { return size_; }
  size_t Capacity() const { return N; }

  void PushBack(const T& value) {
    if (size_ == Capacity())
      throw overflow_error("");
    data_[size_++] = value;
  }
  T PopBack() {
    if (size_ == 0)
      throw underflow_error("");
    return data_[--size_];
  }

 private:
  size_t size_;
  array<T, N> data_;
};
