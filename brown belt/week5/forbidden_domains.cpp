#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>

using namespace std;

bool IsSubdomain(string_view subdomain, string_view domain) {
  //cout << subdomain << " " << domain << endl;
  int i = subdomain.size() - 1;
  int j = domain.size() - 1;
  while (i >= 0 && j >= 0) {
    if (subdomain[i--] != domain[j--]) {
      return false;
    }
  }
  return (i < 0 && j < 0) || (i < 0 && domain[j] == '.')
      || (j < 0 && subdomain[i] == '.');
}

string Reverse(string_view str) {
  return {str.rbegin(), str.rend()};
}

vector<string> ReadDomains(istream& is = cin) {
  size_t count;
  is >> count;

  vector<string> domains(count);
  for (size_t i = 0; i < count; ++i) {
    getline(is, domains[i]);
    if (domains[i].empty())
      --i;
  }

  return domains;
}

void ReverseAll(vector<string>& v) {
  for (string& str : v)
    reverse(str.begin(), str.end());
}

int main() {
  //ifstream fin("input.txt");

  vector<string> banned_domains = ReadDomains();
  const vector<string> domains_to_check = ReadDomains();

  sort(begin(banned_domains), end(banned_domains), [](const string& l, const string& r) {
    return Reverse(l) < Reverse(r);
  });

  size_t insert_pos = 0;
  for (string& domain : banned_domains) {
    if (insert_pos == 0 || !IsSubdomain(banned_domains[insert_pos - 1], domain)) {
      swap(banned_domains[insert_pos++], domain);
    }
  }
  banned_domains.resize(insert_pos);
  ReverseAll(banned_domains);

  for (const string_view domain : domains_to_check) {
    const auto it = upper_bound(begin(banned_domains), end(banned_domains), Reverse(domain));
    if (it != begin(banned_domains) && IsSubdomain(domain, Reverse(*prev(it)))) {
      cout << "Bad" << endl;
    } else {
      cout << "Good" << endl;
    }
  }

  return 0;
}