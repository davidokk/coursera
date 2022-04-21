#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

vector<string_view> SplitIntoWords(string_view line) {
  vector<std::string_view> result;
  size_t curr = line.find_first_not_of(' ', 0);
  while (true) {
    auto space = line.find(' ', curr);
    result.emplace_back(line.substr(curr, space - curr));
    if (space == line.npos)
      break;
    else
      curr = line.find_first_not_of(' ', space);
    if (curr == line.npos)
      break;
  }
  return result;
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;
  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
  {
    lock_guard<std::mutex> lock(index_mutex_);
    swap(new_index, index_);
  }
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  auto func = [&query_input, &search_results_output, this]() {
    vector<size_t> docs;
    vector<size_t> ind;
    docs.resize(50000);
    ind.resize(50000);
    for (string current_query; getline(query_input, current_query); ) {
      size_t curr_ind = 0;
      for (const auto& word : SplitIntoWords(current_query)) {
        vector<std::pair<size_t, size_t>> vec;
        {
          lock_guard<std::mutex> lock(index_mutex_);
          vec = index_.Lookup(word);
        }
        for (const auto& [docid, count] : vec) {
          if (docs[docid] == 0) {
            ind[curr_ind++] = docid;
          }
          docs[docid] += count;
        }
      }

      vector<pair<size_t, size_t>> search_result;
      for (size_t docid = 0; docid < curr_ind; ++docid) {
        size_t count = 0;
        size_t id = 0;
        swap(count, docs[ind[docid]]);
        swap(id, ind[docid]);
        search_result.emplace_back(id, count);
      }

      partial_sort(
          begin(search_result),
          begin(search_result) + min<size_t>(5, search_result.size()),
          end(search_result),
          [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
            int64_t lhs_docid = lhs.first;
            auto lhs_hit_count = lhs.second;
            int64_t rhs_docid = rhs.first;
            auto rhs_hit_count = rhs.second;
            return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
          }
      );

      search_results_output << current_query << ':';
      for (auto [docid, hitcount] : Head(search_result, 5)) {
        search_results_output << " {"
                              << "docid: " << docid << ", "
                              << "hitcount: " << hitcount << '}';
      }
      search_results_output << endl;
    }
  };
  futures_.push_back(async(func));
}

void InvertedIndex::Add(string &&document) {
  docs.push_back(std::move(document));
  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(docs.back())) {
    auto& vec_pair = index[word];
    if (!vec_pair.empty() && vec_pair.back().first == docid) {
      vec_pair.back().second += 1;
    }
    else {
      vec_pair.emplace_back(docid, 1);
    }
  }
}

std::vector<std::pair<size_t, size_t>> InvertedIndex::Lookup(string_view word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}