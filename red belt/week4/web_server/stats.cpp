#include "stats.h"
#include <algorithm>

string_view GetWordAndDelete(string_view& str) {
  int space = str.find(' ');
  string_view word = str.substr(0, space);
  str.remove_prefix(space + 1);
  return word;
}

HttpRequest ParseRequest(string_view line) {
  while (line.front() == ' ')
    line.remove_prefix(1);
  auto method = GetWordAndDelete(line);
  auto uri = GetWordAndDelete(line);
  HttpRequest hr {method, uri, GetWordAndDelete(line)};
  return hr;
}

void Stats::AddMethod(string_view method) {
  auto it = find(METHODS.begin(), METHODS.end(), method);
  if (it == METHODS.end())
    ++methods[UNKNOWN_METHOD];
  else
    ++methods[*it];
}

void Stats::AddUri(string_view uri) {
  auto it = find(URI.begin(), URI.end(), uri);
  if (it == URI.end())
    ++uris[UNKNOWN_URI];
  else
    ++uris[*it];
}

const map<string_view, int>& Stats::GetMethodStats() const { return methods; }
const map<string_view, int>& Stats::GetUriStats() const { return uris; }

Stats::Stats() {
  for (const auto& i : METHODS)
    methods[i] = 0;
  methods[UNKNOWN_METHOD] = 0;
  for (const auto& i : URI)
    uris[i] = 0;
  uris[UNKNOWN_URI] = 0;
}
