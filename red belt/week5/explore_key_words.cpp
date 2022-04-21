#include "test_runner.h"
#include "profile.h"

#include <map>
#include <set>
#include <string>
#include <future>
#include <vector>
#include <functional>
using namespace std;

struct Stats {
  map<string, int> word_frequences;
  void Add(const string& word) { ++word_frequences[word]; }

  void operator += (const Stats& other) {
    for (auto[word, count] : other.word_frequences)
      word_frequences[word] += count;
  }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
  Stats stats;
  istringstream is(line);
  string word;
  while (is >> word) {
    if (key_words.count(word))
      stats.Add(word);
  }
  return stats;
}

Stats ExploreVector(const set<string>& key_words, const vector<string>& lines) {
  Stats result;
  for (const auto& line : lines)
    result += ExploreLine(key_words, line);
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  vector<string> lines;
  vector<future<Stats>> futures;
  string line;
  while (getline(input, line)) {
    lines.push_back(move(line));
    if (lines.size() >= 5000)
      futures.push_back(async(ExploreVector, ref(key_words), move(lines)));
  }
  futures.push_back(async(ExploreVector, ref(key_words), move(lines)));

  Stats total;
  for (auto& f : futures) {
    total += f.get();
  }
  return total;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
      {"yangle", 6},
      {"rocks", 2},
      {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}