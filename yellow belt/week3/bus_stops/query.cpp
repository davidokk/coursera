#include "query.h"

istream& operator >> (istream& is, Query& q) {
    string operation;
    is >> operation;
    if (operation == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int n;
        cin >> n;
        q.stops.clear();
        while (n--) {
            string s;
            is >> s;
            q.stops.push_back(s);
        }
    }
    else if (operation == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }
    else if (operation == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }
    else if (operation == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }

    return is;
}