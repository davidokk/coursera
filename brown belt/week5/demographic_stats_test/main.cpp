#include <iostream>
#include <sstream>

#include "test_runner.h"
//#include "solution.h"

using namespace std;

#define PERSON_AGE(x) Person{x, Gender::FEMALE, true}
void TestComputeMedianAge() {
  vector<Person> persons;
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 0);
  persons.push_back(PERSON_AGE(10));
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 10);
  persons.push_back(PERSON_AGE(5));
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 10);
  persons.push_back(PERSON_AGE(7));
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 7);
};

void TestRead() {
  istringstream input(R"(4
20 0 1
30 1 0
50 1 1
60 0 0)");
  vector<Person> persons = ReadPersons(input);
  AssertEqual(persons[0], Person{20, Gender::FEMALE, true});
  AssertEqual(persons[1], Person{30, Gender::MALE, false});
  AssertEqual(persons[2], Person{50, Gender::MALE, true});
  AssertEqual(persons[3], Person{60, Gender::FEMALE, false});
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 50);
}

void TestRanges() {
  istringstream input(R"(10
10 1 1
20 1 1
30 1 1
25 0 1
34 0 0
37 0 1
12 0 2
64 1 1
53 1 0
34 1 0)");
  AgeStats stats = ComputeStats(ReadPersons(input));
  ASSERT_EQUAL(stats.total, 34);
  ASSERT_EQUAL(stats.females, 34);
  ASSERT_EQUAL(stats.males, 34);
  ASSERT_EQUAL(stats.employed_females, 37);
  ASSERT_EQUAL(stats.unemployed_females, 34);
  ASSERT_EQUAL(stats.employed_males, 30);
  ASSERT_EQUAL(stats.unemployed_males, 53);
}

void TestPrintStats() {
  AgeStats stats {
    .total = 1,
    .females = 2,
    .males = 3,
    .employed_females = 4,
    .unemployed_females = 5,
    .employed_males = 6,
    .unemployed_males = 7
  };
  ostringstream output;
  PrintStats(stats, output);
  AssertEqual(output.str(), R"(Median age = 1
Median age for females = 2
Median age for males = 3
Median age for employed females = 4
Median age for unemployed females = 5
Median age for employed males = 6
Median age for unemployed males = 7
)");
}

int main() {

  TestRunner tr;
  RUN_TEST(tr, TestComputeMedianAge);
  RUN_TEST(tr, TestRead);
  RUN_TEST(tr, TestPrintStats);
  RUN_TEST(tr, TestRanges);
  //PrintStats(ComputeStats(ReadPersons()));
  return 0;
}
