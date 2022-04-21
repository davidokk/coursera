#include <array>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>

using namespace std;

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return {s, nullopt};
  } else {
    return {s.substr(0, pos), s.substr(pos + delimiter.length())};
  }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter = " ") {
  const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
  return {lhs, rhs_opt.value_or("")};
}

string_view ReadToken(string_view& s, string_view delimiter = " ") {
  const auto [lhs, rhs] = SplitTwo(s, delimiter);
  s = rhs;
  return lhs;
}

int ConvertToInt(string_view str) {
  // use std::from_chars when available to git rid of string copy
  size_t pos;
  const int result = stoi(string(str), &pos);
  if (pos != str.length()) {
    std::stringstream error;
    error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
    throw invalid_argument(error.str());
  }
  return result;
}

template <typename Number>
void ValidateBounds(Number number_to_check, Number min_value, Number max_value) {
  if (number_to_check < min_value || number_to_check > max_value) {
    std::stringstream error;
    error << number_to_check << " is out of [" << min_value << ", " << max_value << "]";
    throw out_of_range(error.str());
  }
}

class Date {
 public:
  static Date FromString(string_view str) {
    const int year = ConvertToInt(ReadToken(str, "-"));
    const int month = ConvertToInt(ReadToken(str, "-"));
    ValidateBounds(month, 1, 12);
    const int day = ConvertToInt(str);
    ValidateBounds(day, 1, 31);
    return {year, month, day};
  }

  static int RND(Date date) {
    if (date.month_ < 3)
      date.year_--, date.month_ += 12;
    return 365*date.year_ + date.year_/4 - date.year_/100 + date.year_/400 + (153*date.month_ - 457)/5 + date.day_ - 306;
  }

 private:
  int year_;
  int month_;
  int day_;

  Date(int year, int month, int day)
      : year_(year), month_(month), day_(day)
  {}
};

int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
  return Date::RND(date_to) - Date::RND(date_from);
}


static const Date START_DATE = Date::FromString("2000-01-01");
static const Date END_DATE = Date::FromString("2100-01-01");
static const size_t DAY_COUNT = ComputeDaysDiff(END_DATE, START_DATE);
static const size_t DAY_COUNT_P2 = 1 << 16;
static const size_t VERTEX_COUNT = DAY_COUNT_P2 * 2;

size_t ComputeDayIndex(const Date& date) {
  return ComputeDaysDiff(date, START_DATE);
}


array<double, VERTEX_COUNT> tree_values, tree_add, tree_factor, tree_spending, tree_spending_values;

void Init() {
  tree_values.fill(0);
  tree_add.fill(0);
  tree_factor.fill(1);
  tree_spending.fill(0);
}

void Push(size_t v, size_t l, size_t r) {
  for (size_t w = v * 2; w <= v * 2 + 1; ++w) {
    if (w < VERTEX_COUNT) {
      tree_spending[w] += tree_spending[v];
      tree_factor[w] *= tree_factor[v];
      (tree_add[w] *= tree_factor[v]) += tree_add[v];
      (tree_values[w] *= tree_factor[v]) += tree_add[v] * (r - l) / 2;
      tree_spending_values[w] += tree_spending[v] * (r - l) / 2;
    }
  }
  tree_factor[v] = 1;
  tree_add[v] = 0;
  tree_spending[v] = 0;
}

double ComputeSum(size_t v, size_t l, size_t r, size_t ql, size_t qr) {
  if (v >= VERTEX_COUNT || qr <= l || r <= ql) {
    return 0;
  }
  Push(v, l, r);
  if (ql <= l && r <= qr) {
    return tree_values[v] - tree_spending_values[v];
  }
  return ComputeSum(v * 2, l, (l + r) / 2, ql, qr)
      + ComputeSum(v * 2 + 1, (l + r) / 2, r, ql, qr);
}

void Add(size_t v, size_t l, size_t r, size_t ql, size_t qr, double value) {
  if (v >= VERTEX_COUNT || qr <= l || r <= ql) {
    return;
  }
  Push(v, l, r);
  if (ql <= l && r <= qr) {
    if (value > 0) {
      tree_add[v] += value;
      tree_values[v] += value * (r - l);
    } else {
      tree_spending[v] -= value;
      tree_spending_values[v] -= value * (r - l);
    }
    return;
  }
  Add(v * 2, l, (l + r) / 2, ql, qr, value);
  Add(v * 2 + 1, (l + r) / 2, r, ql, qr, value);
  if (value > 0)
    tree_values[v] =
        (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : 0)
            + (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : 0);
  else
    tree_spending_values[v] =
        (v * 2 < VERTEX_COUNT ? tree_spending_values[v * 2] : 0)
            + (v * 2 + 1 < VERTEX_COUNT ? tree_spending_values[v * 2 + 1] : 0);
}

void Multiply(size_t v, size_t l, size_t r, size_t ql, size_t qr, double value) {
  if (v >= VERTEX_COUNT || qr <= l || r <= ql) {
    return;
  }
  Push(v, l, r);
  if (ql <= l && r <= qr) {
    tree_factor[v] *= value;
    tree_add[v] *= value;
    tree_values[v] *= value;
    return;
  }
  Multiply(v * 2, l, (l + r) / 2, ql, qr, value);
  Multiply(v * 2 + 1, (l + r) / 2, r, ql, qr, value);
  tree_values[v] =
      (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : 0)
          + (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : 0);
}


int main() {
  cout.precision(25);
  assert(DAY_COUNT <= DAY_COUNT_P2 && DAY_COUNT_P2 < DAY_COUNT * 2);

  Init();

  int q;
  cin >> q;

  for (int i = 0; i < q; ++i) {
    string query_type;
    cin >> query_type;

    string date_from_str, date_to_str;
    cin >> date_from_str >> date_to_str;

    auto idx_from = ComputeDayIndex(Date::FromString(date_from_str));
    auto idx_to = ComputeDayIndex(Date::FromString(date_to_str)) + 1;

    if (query_type == "ComputeIncome") {
      cout << ComputeSum(1, 0, DAY_COUNT_P2, idx_from, idx_to) << endl;
    } else if (query_type == "PayTax") {
      int percent;
      cin >> percent;
      Multiply(1, 0, DAY_COUNT_P2, idx_from, idx_to, 1 - percent * 1.0 / 100);
    } else if (query_type == "Earn") {
      double value;
      cin >> value;
      Add(1, 0, DAY_COUNT_P2, idx_from, idx_to, value / (idx_to - idx_from));
    } else if (query_type == "Spend") {
      double value;
      cin >> value;
      Add(1, 0, DAY_COUNT_P2, idx_from, idx_to, - value / (idx_to - idx_from));
    }
  }

  return 0;
}