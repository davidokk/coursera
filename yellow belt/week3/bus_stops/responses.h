#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

struct BusesForStopResponse {
    vector <string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
    vector <pair <string, vector <string>>> stops;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
    vector <pair <string, vector <string>>> buses;
};

ostream& operator << (ostream& os, const AllBusesResponse& r);