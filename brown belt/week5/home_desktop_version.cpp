#include <iostream>
#include <ctime>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>

class Date {
 public:
  Date() : year_(1970), month_(1), day_(1) {}
  Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

  // Rata Die day one is 0001-01-01
  static int RND(Date date) {
    if (date.month_ < 3)
      date.year_--, date.month_ += 12;
    return 365*date.year_ + date.year_/4 - date.year_/100 + date.year_/400 + (153*date.month_ - 457)/5 + date.day_ - 306;
  }
  static int ComputeDaysDiff(const Date& date_from,  const Date& date_to) {
    return RND(date_to) - RND(date_from);
  }

  friend std::istream& operator>>(std::istream& in, Date& date) {
    std::string line;
    std::getline(in, line, '-');
    date.year_ = std::stoi(line);
    std::getline(in, line, '-');
    date.month_ = std::stoi(line);
    std::getline(in, line, ' ');
    date.day_ = std::stoi(line);
    return in;
  }

 private:
  int year_, month_, day_;
};

enum class RequestType {
  Earn, ComputeIncome, PayTax, Spend, Undefined
};

struct Request {
  RequestType type_ = RequestType::Undefined;
  Date from;
  Date to;
  int value{};
};

class FinancialManager {
 public:
  FinancialManager() : tree(4 * N) {}

  double ComputeIncome(const Date& from, const Date& to) const {
    return Sum(DayNum(from), DayNum(to));
  }

  void Earn(const Date& from, const Date& to, double value) {
    double earnings = value / (Date::ComputeDaysDiff(from, to) + 1);
    Update(DayNum(from), DayNum(to), [earnings](double& val){ val += earnings; });
  }

  void PayTax(const Date& from, const Date& to) {
    Update(DayNum(from), DayNum(to), [=](double& val){ val *= 0.87; });
  }

 private:
  double Sum(int l, int r) const { return Sum(1, 0, N, l, r); }

  double Sum(int v, int tl, int tr, int l, int r) const {
    if (l > r)
      return 0;
    if (l == tl && r == tr)
      return tree[v];
    int m = (tl + tr) / 2;
    return Sum(2 * v, tl, m, l, std::min(r, m)) + Sum(2 * v + 1, m + 1, tr, std::max(l, m + 1), r);
  }

  template <typename Func>
  void Update(int l, int r, Func func) { Update(1, 0, N, l, r, func); }

  template <typename Func>
  void Update(int v, int tl, int tr, int l, int r, Func func) {
    if (tl > r || tr < l)
      return;
    if (tl == tr) {
      func(tree[v]);
      return;
    }
    int m = (tl + tr) / 2;
    Update(2 * v, tl, m, l, r, func);
    Update(2 * v + 1, m + 1, tr, l, r, func);
    tree[v] = tree[2 * v] + tree[2 * v + 1];
  }

  int DayNum(const Date& date) const { return Date::ComputeDaysDiff(first_day, date); }

 private:
  const Date first_day = Date(2000, 1, 1);
  const Date last_day = Date(2099, 12, 31);
  const int N = Date::ComputeDaysDiff(first_day, last_day);
  std::vector<double> tree;
};

class RequestHandler {
 public:
  template<typename InputIt, typename OutputIt>
  void ExecuteRequest(InputIt begin, InputIt end, OutputIt out) {
    for (auto it = begin; it != end; ++it) {
      switch (it->type_) {
        case RequestType::Earn:
          manager.Earn(it->from, it->to, it->value);
          break;
        case RequestType::ComputeIncome:
          *out++ = manager.ComputeIncome(it->from, it->to);
          break;
        case RequestType::PayTax:
          manager.PayTax(it->from, it->to);
          break;
        default:
          throw std::invalid_argument("undefined request type");
      }
    }
  }
 private:
  FinancialManager manager;
};


Request ParseRequest(const std::string& str) {
  Request result;
  std::istringstream ss(str);
  std::string type;
  ss >> type;
  if (type == "Earn")
    result.type_ = RequestType::Earn;
  else if (type == "ComputeIncome")
    result.type_ = RequestType::ComputeIncome;
  else if (type == "PayTax")
    result.type_ = RequestType::PayTax;
  else if (type == "Spend")
    result.type_ = RequestType::Spend;
  ss >> result.from >> result.to;
  if (type == "Earn")
    ss >> result.value;
  return result;
}

std::vector<Request> ReadRequest(std::istream& in) {
  std::vector<Request> result;
  int request_count;
  in >> request_count;
  result.reserve(request_count);

  std::string line;
  while (request_count != result.size()) {
    getline(in, line);
    if (!line.empty())
      result.push_back(ParseRequest(line));
  }
  return result;
}

int main() {

  //std::ifstream in("input.txt");
  auto requests = ReadRequest(std::cin);
  std::cout.precision(25);
  RequestHandler().ExecuteRequest(requests.begin(), requests.end(), std::ostream_iterator<double>(std::cout, "\n"));

  return 0;
}
