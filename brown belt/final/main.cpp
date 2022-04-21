//#include "test_runner.h"

#include "Request.h"

#include <iostream>
#include <fstream>

int main() {

  // std::ifstream json_input("json.txt");
  auto requests = ReadRequestsFromJson(std::cin);
  auto responses = ProcessRequests(requests);

  // std::ofstream out("out.txt");
  PrintResponsesInJson(responses, std::cout);

  return 0;
}
