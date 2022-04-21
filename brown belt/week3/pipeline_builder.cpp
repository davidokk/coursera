#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Email {
  string from;
  string to;
  string body;
};

class Worker {
 public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    throw logic_error("Unimplemented");
  }

 protected:
  void PassOn(unique_ptr<Email> email) const {
    if (next_worker)
      next_worker->Process(move(email));
  }

 public:
  void SetNext(unique_ptr<Worker> next) {
    next_worker = move(next);
  }

 private:
  unique_ptr<Worker> next_worker;
};

class Reader : public Worker {
 public:
  explicit Reader(istream& input) : in(input) {}

  void Process(unique_ptr<Email>) override {}
  void Run() override {
    while (in) {
      string from, to, body;
      getline(in, from);
      getline(in, to);
      getline(in, body);
      if (in)
        PassOn(make_unique<Email>(Email{from, to, body}));
    }
  }

 private:
  istream& in;
};

class Filter : public Worker {
 public:
  using Function = function<bool(const Email&)>;

  explicit Filter(Function f) : func(std::move(f)) {}
  void Process(unique_ptr<Email> email) override {
    if (func(*email))
      PassOn(move(email));
  }

 private:
  Function func;
};

class Copier : public Worker {
 public:
  explicit Copier(string to) : to_(move(to)) {}

  void Process(unique_ptr<Email> email) override {
    unique_ptr<Email> copied;
    if (to_ != email->to)
      copied = make_unique<Email>(Email{email->from, to_, email->body});
    PassOn(move(email));
    if (copied)
      PassOn(move(copied));
  }

 private:
  string to_;
};

class Sender : public Worker {
 public:
  explicit Sender(ostream& output) : out(output) {}

  void Process(unique_ptr<Email> email) override {

    out << email->from << endl << email->to << endl << email->body << endl;
    PassOn(move(email));
  }

 private:
  ostream& out;
};

class PipelineBuilder {
 public:
  explicit PipelineBuilder(istream& in) {
    chain.push_back(make_unique<Reader>(in));
  }

  PipelineBuilder& FilterBy(Filter::Function filter) {
    chain.push_back(make_unique<Filter>(move(filter)));
    return *this;
  }

  PipelineBuilder& CopyTo(string recipient) {
    chain.push_back(make_unique<Copier>(move(recipient)));
    return *this;
  }

  PipelineBuilder& Send(ostream& out) {
    chain.push_back(make_unique<Sender>(out));
    return *this;
  }

  unique_ptr<Worker> Build() {
    for (size_t i = chain.size() - 1; i > 0; --i)
      chain[i - 1]->SetNext(move(chain[i]));
    return move(chain[0]);
  }

 private:
  vector<unique_ptr<Worker>> chain;
};

void TestSanity() {
  string input = (
      "erich@example.com\n"
      "richard@example.com\n"
      "Hello there\n"

      "erich@example.com\n"
      "ralph@example.com\n"
      "Are you sure you pressed the right button?\n"

      "ralph@example.com\n"
      "erich@example.com\n"
      "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
      "erich@example.com\n"
      "richard@example.com\n"
      "Hello there\n"

      "erich@example.com\n"
      "ralph@example.com\n"
      "Are you sure you pressed the right button?\n"

      "erich@example.com\n"
      "richard@example.com\n"
      "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}