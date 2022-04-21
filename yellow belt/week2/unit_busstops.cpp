#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

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

struct BusesForStopResponse {
    vector <string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty())
        os << "No stop";
    else
        for (auto& item : r.buses)
            os << item << " ";
    return os;
}

struct StopsForBusResponse {
    vector <pair <string, vector <string>>> stops;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    if (r.stops.empty())
        os << "No bus";
    else {
        bool first = true;
        for (auto& [stop, buses] : r.stops) {
            if (!first)
                cout << endl;
            first = false;
            cout << "Stop " << stop << ": ";
            if (buses.size() == 0)
                cout << "no interchange";
            else
                for (auto bus : buses)
                    cout << bus << " ";
        }
    }
    return os;
}

struct AllBusesResponse {
    vector <pair <string, vector <string>>> buses;

};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    if (r.buses.empty())
        cout << "No buses";
    else {
        bool first = true;
        for (auto&[bus, stops] : r.buses) {
            if (!first)
                cout << endl;
            first = false;
            cout << "Bus " << bus << ": ";
            for (auto stop : stops)
                cout << stop << " ";
        }
    }
    return os;
}

class BusManager {
private:
    map <string, vector <string>> buses, stops;
public:
    void AddBus(const string& bus, const vector<string>& stops_v) {
        for (auto& stop : stops_v) {
            stops[stop].push_back(bus);
            buses[bus].push_back(stop);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        if (stops.count(stop) == 0)
            return {};
        return BusesForStopResponse{stops.at(stop)};
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        vector <pair <string, vector <string>>> t;
        if (buses.count(bus) == 0)
            return {};
        for (auto& stop : buses.at(bus)) {
            vector <string> p;
            for (auto& cur_bus : stops.at(stop))
                if (bus != cur_bus)
                    p.push_back(cur_bus);
            t.push_back(make_pair(stop, p));
        }
        return StopsForBusResponse{t};
    }

    AllBusesResponse GetAllBuses() const {
        vector <pair <string, vector <string>>> t;
        for (auto& [bus, cur_stops] : buses) {
            t.push_back(make_pair(bus, cur_stops));
        }
        return AllBusesResponse{t};
    }
};

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

    return 0;
}