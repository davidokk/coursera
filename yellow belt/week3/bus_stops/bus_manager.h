#pragma once

#include <string>
#include <vector>
#include <map>
#include "responses.h"

using namespace std;

class BusManager {
private:
    map <string, vector <string>> buses, stops;
public:
    void AddBus(const string& bus, const vector<string>& stops_v);

    BusesForStopResponse GetBusesForStop(const string& stop) const;

    StopsForBusResponse GetStopsForBus(const string& bus) const;

    AllBusesResponse GetAllBuses() const;
};