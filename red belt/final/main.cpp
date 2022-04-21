#include "tests.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <thread>
using namespace std;

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSerpFormat);
  RUN_TEST(tr, TestTop5);
  RUN_TEST(tr, TestHitcount);
  RUN_TEST(tr, TestRanking);
  RUN_TEST(tr, TestBasicSearch);

   // {
   //   LOG_DURATION("generate big test");
   //   GenerateTest(1000, 1000);
   // }
   // LOG_DURATION("big test");
   // RUN_TEST(tr, BigTest);
}
