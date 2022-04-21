#include <iostream>

using namespace std;

#define UNIQ_2(line) var ## line
#define UNIQ(line) UNIQ_2(line)
#define UNIQ_ID UNIQ(__LINE__)

int main() {

  int UNIQ_ID = 3;

  return 0;
}
