#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Table {
 public:
  Table(size_t n, size_t m) {
    if (n == 0 || m == 0)
      n = m = 0;
      Resize(n, m);
  }

  void Resize(size_t n, size_t m) {
    t.resize(n);
    for (vector<T>& row : t)
      row.resize(m);
  }

  vector<T>& operator[] (size_t row) {
    return t[row];
  }
  const vector<T>& operator[] (size_t row) const {
    return t[row];
  }

  pair<size_t, size_t> Size() const {
    if (t.empty())
      return make_pair(0, 0);
    return make_pair(t.size(), t[0].size());
  }

 private:
  vector<vector<T>> t;
};

int main() {

  return 0;
}
