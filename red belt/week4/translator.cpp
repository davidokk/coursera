#include "test_runner.h"
#include <string>
#include <vector>
#include <deque>

using namespace std;

class Translator {
 public:
  void Add(string_view source, string_view target) {
    lang12[source] = AddCloneView(target);
    lang21[target] = AddCloneView(source);
  }
  string_view TranslateForward(string_view source) const {
    if (lang12.count(source))
      return lang12.at(source);
    return {};
  }
  string_view TranslateBackward(string_view target) const {
    if (lang21.count(target))
      return lang21.at(target);
    return {};
  }

 private:
  string_view AddCloneView(string_view sv) {
    for (auto mp : {&lang12, &lang21}) {
      auto it = mp->find(sv);
      if (it != mp->end())
        return it->first;
    }
    return strings.emplace_back(sv);
  }

  map<string_view, string_view> lang12;
  map<string_view, string_view> lang21;
  deque<string_view> strings;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}