#pragma once

#include <cstdlib>

// Р РµР°Р»РёР·СѓР№С‚Рµ С€Р°Р±Р»РѕРЅ SimpleVector
template <typename T>
class SimpleVector {
 public:
  SimpleVector() : capacity_(0), size_(0), data(nullptr) {}
  explicit SimpleVector(size_t size) : capacity_(size), size_(size) {
    data = new T[size];
  }
  ~SimpleVector() { delete[] data; }

  T& operator[](size_t index) { return data[index]; }

  T* begin() { return data; }
  T* end() { return data + size_; }
  const T* begin() const { return data; }
  const T* end() const { return data + size_; }

  size_t Size() const { return size_; }
  size_t Capacity() const { return capacity_; }
  void PushBack(const T& value) {
    if (!data) {
      data = new T[1];
      capacity_ = 1;
    }
    if (size_ == capacity_)
      Extend();
    data[size_++] = value;
  }

 private:
  void Extend() {
    T* new_data = new T[2 * capacity_];
    for (int i = 0; i < size_; i++)
      new_data[i] = data[i];
    delete[] data;
    data = new_data;
    capacity_ *= 2;
  }

  int capacity_;
  int size_;
  T* data;
};