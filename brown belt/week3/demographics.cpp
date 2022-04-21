#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <optional>

using namespace std;

template <typename Iterator>
class IteratorRange {
 public:
  IteratorRange(Iterator begin, Iterator end)
      : first(begin)
      , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

 private:
  Iterator first, last;
};

template <typename Iterator>
auto MakeRange(Iterator begin, Iterator end) {
  return IteratorRange<Iterator>(begin, end);
}

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return MakeRange(v.begin(), next(v.begin(), min(top, v.size())));
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

vector<Person> ReadPeople(istream& input) {
  int count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = (gender == 'M');
  }

  return result;
}

int AdultCount(const vector<Person>& people, int adult_age) {
  auto adult_begin = lower_bound(
      begin(people), end(people), adult_age, [](const Person& lhs, int age) {
        return lhs.age < age;
      }
  );
  return distance(adult_begin, people.end());
}

int TotalIncome(const vector<Person>& people, int count) {
  int income = 0;
  for (const Person& p : Head(people, count))
    income += p.income;
  return income;
}

template<typename K, typename V>
optional<K> MaxValueInMap(const map<K, V>& m) {
  optional<K> key;
  optional<V> value;
  for (auto&[k, v] : m) {
    if (!key || v > value) {
      key = k;
      value = v;
    }
  }
  return key;
}

pair<string, string> PopularNames(const vector<Person>& people) {
  vector<map<string, int>> mf(2);
  for (const Person& p : people)
    ++mf[p.is_male][p.name];
  auto m = MaxValueInMap(mf[0]);
  auto f = MaxValueInMap(mf[1]);
  return {m? "Most popular name among people of gender W is " + m.value() : "No people of gender W",
          f? "Most popular name among people of gender M is " + f.value() : "No people of gender M"};
}

void Solve(istream& input, ostream& output) {
  const vector<Person> people_sorted_by_age = [&]{
    vector<Person> p = ReadPeople(input);
    sort(begin(p), end(p), [](const Person& lhs, const Person& rhs) {
      return lhs.age < rhs.age;
    });
    return p;
  }();

  const vector<Person> people_sorted_by_income = [&]{
    vector<Person> p(people_sorted_by_age);
    sort(begin(p), end(p), [](const Person& lhs, const Person& rhs) {
      return lhs.income > rhs.income;
    });
    return p;
  }();

  const pair<string, string> popular_names = PopularNames(people_sorted_by_age);

  for (string command; input >> command; ) {
    if (command == "AGE") {
      int adult_age; input >> adult_age;
      output << "There are " << AdultCount(people_sorted_by_age, adult_age)
           << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY") {
      int count; input >> count;
      output << "Top-" << count << " people have total income " << TotalIncome(people_sorted_by_income, count) << '\n';
    } else if (command == "POPULAR_NAME") {
      char gender;
      input >> gender;
      output << (gender == 'W' ? popular_names.first : popular_names.second) << '\n';
    }
  }
}

void Test() {
  {
    istringstream input(R"(11
Ivan 25 1000 M
Olga 30 623 W
Sergey 24 825 M
Maria 42 1254 W
Mikhail 15 215 M
Oleg 18 230 M
Denis 53 8965 M
Maxim 37 9050 M
Ivan 47 19050 M
Ivan 17 50 M
Olga 23 550 W
AGE 18
AGE 25
WEALTHY 5
POPULAR_NAME M)");
    ostringstream output;

    Solve(input, output);

    string expected = R"(There are 9 adult people for maturity age 18
There are 6 adult people for maturity age 25
Top-5 people have total income 39319
Most popular name among people of gender M is Ivan
)";

    ASSERT_EQUAL(output.str(), expected);
  }

  {
    istringstream input(R"(0
AGE 15
AGE 45
WEALTHY 100
POPULAR_NAME M
POPULAR_NAME W)");
    ostringstream output;

    Solve(input, output);

    string expected = R"(There are 0 adult people for maturity age 15
There are 0 adult people for maturity age 45
Top-100 people have total income 0
No people of gender M
No people of gender W
)";

    ASSERT_EQUAL(output.str(), expected);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  //Solve(cin, cout);
}