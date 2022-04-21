#include "Common.h"

#include <list>
#include <unordered_map>
#include <mutex>

using namespace std;

class LruCache : public ICache {
 public:
  LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings& settings_)
    : settings(settings_), unpacker(move(books_unpacker)) {}

  BookPtr GetBook(const string& book_name) override {
    lock_guard<mutex> g(m);
    if (lru.count(book_name)) {
      history.splice(history.begin(), history, lru[book_name].second);
      return lru[history.front()].first;
    } else {
      history.push_front(book_name);
      BookPtr book = unpacker->UnpackBook(history.front());
      size += book->GetContent().size();
      lru[book_name] = {book, history.begin()};
      CheckSize();
      return book;
    }
  }

  void CheckSize() {
    while (size > settings.max_memory && !history.empty()) {
      const std::string book_name = history.back();
      history.pop_back();
      size -= lru[book_name].first->GetContent().size();
      lru.erase(book_name);
    }
  }

 private:
  const Settings settings;
  shared_ptr<IBooksUnpacker> unpacker;

  mutable mutex m;

  size_t size = 0;
  unordered_map<string, pair<BookPtr, list<string>::iterator>> lru;
  list<string> history;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(books_unpacker, settings);
}