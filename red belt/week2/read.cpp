#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
 public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        pref_pages(MAX_PAGES_COUNT_ + 1, 0),
        total_users(0) {}

  void Read(int user_id, int page_count) {
    if (user_page_counts_[user_id] == 0) {
      ++total_users;
      for (int i = page_count; i <= MAX_PAGES_COUNT_; ++i)
        ++pref_pages[i];
    } else {
      for (int i = user_page_counts_[user_id]; i < page_count; ++i)
        --pref_pages[i];
    }
    user_page_counts_[user_id] = page_count;
  }

  double Cheer(int user_id) const {
    if (total_users == 0 || user_page_counts_[user_id] == 0)
      return 0;
    if (total_users == 1)
      return 1;
    return 1.0 * (pref_pages[user_page_counts_[user_id] - 1]) / (total_users - 1);
  }

 private:
  static constexpr int MAX_USER_COUNT_ = 100'000;
  static constexpr int MAX_PAGES_COUNT_ = 1'000;

  vector<int> user_page_counts_;
  vector<int> pref_pages;

  int total_users;
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << endl;
    }
  }

  return 0;
}