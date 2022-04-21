#pragma once

#include <iostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <future>
#include <mutex>
#include <deque>
#include <thread>

using namespace std;

class InvertedIndex {
 public:
  void Add(string &&document);
  std::vector<std::pair<size_t, size_t>> Lookup(string_view word) const;

  const string& GetDocument(size_t id) const { return docs[id]; }

 private:
  map<std::string_view, std::vector<std::pair<size_t, size_t>>> index;
  std::deque<string> docs;
};

class SearchServer {
 public:
  SearchServer() = default;
  ~SearchServer() {
    for (auto& f : futures_)
      f.get();
  }
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);
 private:
  std::vector<std::future<void>> futures_;
  std::mutex index_mutex_;
  InvertedIndex index_;
};