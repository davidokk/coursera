#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
using namespace std;

class Stats {
 public:
  Stats();
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;
 private:
  map<string_view, int> methods;
  map<string_view, int> uris;

  const array<string, 4> METHODS{"GET", "POST", "PUT", "DELETE"};
  const array<string, 5> URI{"/", "/order", "/product", "/basket", "/help"};
  const string UNKNOWN_METHOD = "UNKNOWN";
  const string UNKNOWN_URI = "unknown";
};

HttpRequest ParseRequest(string_view line);