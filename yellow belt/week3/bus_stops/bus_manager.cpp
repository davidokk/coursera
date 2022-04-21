#include "bus_manager.h"

    void BusManager::AddBus(const string& bus, const vector<string>& stops_v) {
        for (auto &stop : stops_v) {
            stops[stop].push_back(bus);
            buses[bus].push_back(stop);
        }
    }

    BusesForStopResponse BusManager::GetBusesForStop(const string& stop) const {
        if (stops.count(stop) == 0)
            return {};
        return BusesForStopResponse{stops.at(stop)};
    }

    StopsForBusResponse BusManager::GetStopsForBus(const string& bus) const {
        vector<pair<string, vector<string>>> t;
        if (buses.count(bus) == 0)
            return {};
        for (auto &stop : buses.at(bus)) {
            vector<string> p;
            for (auto &cur_bus : stops.at(stop))
                if (bus != cur_bus)
                    p.push_back(cur_bus);
            t.push_back(make_pair(stop, p));
        }
        return StopsForBusResponse{t};
    }

    AllBusesResponse BusManager::GetAllBuses() const {
        vector<pair<string, vector<string>>> t;
        for (auto&[bus, cur_stops] : buses) {
            t.push_back(make_pair(bus, cur_stops));
        }
        return AllBusesResponse{t};
    }
