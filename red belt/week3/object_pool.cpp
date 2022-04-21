#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
 public:
  T* Allocate() {
    if (!deallocated.empty()) {
      T* temp = deallocated.front();
      allocated.insert(temp);
      deallocated.pop();
      return temp;
    } else {
      T* temp = new T;
      allocated.insert(temp);
      return temp;
    }
  }
  T* TryAllocate() {
    if (!deallocated.empty())
      return Allocate();
    else
      return nullptr;
  }

  void Deallocate(T* object) {
    auto it = allocated.find(object);
    if (it != allocated.end()) {
      deallocated.push(object);
      allocated.erase(it);
    } else {
      throw invalid_argument("");
    }
  }

  ~ObjectPool() {
    while (!deallocated.empty()) {
      delete deallocated.front();
      deallocated.pop();
    }
    while (!allocated.empty()) {
      delete *allocated.begin();
      allocated.erase(allocated.begin());
    }
  }

 private:
  set<T*> allocated;
  queue<T*> deallocated;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}