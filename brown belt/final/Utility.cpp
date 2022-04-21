#include "Utility.h"

#include <iostream>
#include <optional>
#include <cmath>

using namespace std;

const double pi = acos(-1);

double DegToRad(double deg) {
  return deg * pi / 180;
}

double ComputeDistance(const Coordinates& lhs, const Coordinates& rhs) {
  double llat = DegToRad(lhs.latitude), rlat = DegToRad(rhs.latitude);
  double llon = DegToRad(lhs.longitude), rlon = DegToRad(rhs.longitude);
  return acos(sin(llat) * sin(rlat) + cos(llat) * cos(rlat) * cos(abs(llon - rlon))) * 6371000;
}

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return {s, nullopt};
  } else {
    return {s.substr(0, pos), s.substr(pos + delimiter.length())};
  }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter) {
  const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
  return {lhs, rhs_opt.value_or("")};
}

string_view DeleteSpaces(string_view str) {
  while (str.front() == ' ')
    str.remove_prefix(1);
  while (str.back() == ' ')
    str.remove_suffix(1);
  return str;
}

string_view ReadToken(string_view& s, string_view delimiter) {
  const auto [lhs, rhs] = SplitTwo(s, delimiter);
  s = rhs;
  return lhs;
}
