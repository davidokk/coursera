#include <cstdint>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector {
 public:
  SimpleVector() : size_(0), capacity_(0), data_(nullptr) {}
  explicit SimpleVector(size_t size) : data_(new T[size]), size_(size), capacity_(size) {}

  ~SimpleVector() { delete[] data_; }

  T& operator[](size_t index) { return data_[index]; }
  const T& operator[](size_t index) const { return data_[index]; }

  T* begin() { return data_; }
  T* end() { return data_ + size_; }
  const T* begin() const { return data_; }
  const T* end() const { return data_ + size_; }

  size_t Size() const { return size_; }
  size_t Capacity() const { return capacity_; }

  void PushBack(const T& value) {
    if (size_ == capacity_)
      Reallocate();
    data_[size_++] = value;
  }

  void PushBack(T&& value) {
    if (size_ == capacity_)
      Reallocate();
    data_[size_++] = move(value);
  }

 private:
  void Reallocate() {
    if (capacity_) {
      capacity_ *= 2;
    } else {
      capacity_ = 1;
    }
    T* new_data = new T[capacity_];
    move(begin(), end(), new_data);
    delete[] data_;
    data_ = new_data;
  }

  T* data_;
  size_t size_;
  size_t capacity_;
};