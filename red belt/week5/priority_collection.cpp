#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
 public:
  using Id = int;
  struct PriorityObject {
    T data;
    int priority;
  };

  Id Add(T object) {
    Id id = objects.size();
    objects.push_back({move(object), 0});
    sorted_objects.insert({0, id});
    return id;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
    while (range_begin != range_end)
      *ids_begin++ = Add(move(*range_begin++));
  }

  bool IsValid(Id id) const {
    return id >= 0 && id < objects.size() && objects[id].priority != -1;
  }

  const T& Get(Id id) const {
    return objects[id].data;
  }

  void Promote(Id id) {
    sorted_objects.erase({objects[id].priority, id});
    ++objects[id].priority;
    sorted_objects.insert({objects[id].priority, id});
  }

  pair<const T&, int> GetMax() const {
    auto it = prev(sorted_objects.end());
    return {objects[it->second].data, it->first};
  }

  pair<T, int> PopMax() {
    auto it = prev(sorted_objects.end());
    pair<T, int> p = make_pair(move(objects[it->second].data), it->first);
    objects[it->second].priority = -1;
    sorted_objects.erase(it);
    return p;
  }

 private:
  vector<PriorityObject> objects;
  set<pair<int, Id>> sorted_objects;
};


class StringNonCopyable : public string {
 public:
  using string::string;
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}