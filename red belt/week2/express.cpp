#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

class RouteManager {
 public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].insert(finish);
    reachable_lists_[start].insert(start);
    reachable_lists_[finish].insert(start);
    reachable_lists_[finish].insert(finish);
  }
  int FindNearestFinish(int start, int finish) const {
    if (reachable_lists_.count(start) == 0)
      return abs(finish - start);

    if (reachable_lists_.at(start).count(finish) == 1)
      return 0;

    const set<int>& routes = reachable_lists_.at(start);

    auto it = routes.upper_bound(finish);

    if (it == routes.end())
      return abs(finish - *prev(it));
    if (it == routes.begin())
      return abs(finish - *it);

    return min(abs(finish - *it), abs(finish - *prev(it)));
  }
 private:
  map<int, set<int>> reachable_lists_;
};

int main() {
  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}