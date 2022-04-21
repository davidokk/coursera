#include <iostream>
#include <list>
#include <map>
#include <fstream>

using namespace std;

int main() {
  // ifstream cin("input.txt");

  int n;
  cin >> n;

  list<int> order;
  map<int, list<int>::iterator> positions;

  while (n--) {
    int number, before;
    cin >> number >> before;
    if (positions.count(before) == 0) {
      order.push_back(number);
      positions[number] = prev(order.end());
    }
    else
      positions[number] = order.insert(positions[before], number);
  }

  for (const auto& i : order)
    cout << i << "\n";

  return 0;
}
