#pragma once

#include <iostream>
#include <sstream>

struct Coordinates {
  double latitude;
  double longitude;
};

double ComputeDistance(const Coordinates&, const Coordinates&);

std::string_view ReadToken(std::string_view&, std::string_view delimiter = " ");
std::string_view DeleteSpaces(std::string_view);

std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");

template <typename Number>
Number ConvertTo(std::string_view str) {
  std::stringstream ss;
  ss << str;
  Number num;
  ss >> num;
  return num;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& stream) {
  Number number;
  stream >> number;
  std::string dummy;
  getline(stream, dummy);
  return number;
}

