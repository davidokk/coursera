#include <iostream>
#include <vector>

#include "deque.h"

using namespace std;

int main() {
  Deque<int> d;
  d.PushFront(1);
  d.PushFront(2);

  cout << d.Back();
  return 0;
}
