#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

class Database {
 public:
  bool Put(const Record& record) {
    auto [it, is_inserted] = by_id.insert({record.id, Location{record, {}, {}, {}}});
    if (!is_inserted)
      return false;

    auto& data = it->second;
    const Record* ptr = &data.record;
    data.timestamp_it = by_timestamp.insert({record.timestamp, ptr});
    data.karma_it = by_karma.insert({record.karma, ptr});
    data.user_it = by_user.insert({record.user, ptr});
    return true;
  }

  const Record* GetById(const string& id) const {
    auto it = by_id.find(id);
    if (it == by_id.end())
      return nullptr;
    return &it->second.record;
  }

  bool Erase(const string& id) {
    auto it = by_id.find(id);
    if (it == by_id.end())
      return false;

    by_user.erase(it->second.user_it);
    by_timestamp.erase(it->second.timestamp_it);
    by_karma.erase(it->second.karma_it);
    by_id.erase(it);
    return true;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    Run(by_timestamp.lower_bound(low), by_timestamp.upper_bound(high), callback);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    Run(by_karma.lower_bound(low), by_karma.upper_bound(high), callback);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    Run(by_user.lower_bound(user), by_user.upper_bound(user), callback);
  }

 private:
  template <typename T>
  using Index = multimap<T, const Record*>;

  struct Location {
    Record record;
    Index<int>::iterator timestamp_it;
    Index<int>::iterator karma_it;
    Index<string>::iterator user_it;
  };

  unordered_map<string, Location> by_id;
  Index<int> by_timestamp;
  Index<int> by_karma;
  Index<string> by_user;

  template<typename Iterator, typename CallBack>
  void Run(Iterator begin, Iterator end, CallBack callback) const {
    for (auto it = begin; it != end; ++it)
      if (!callback(*(it->second)))
        break;
  }
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}