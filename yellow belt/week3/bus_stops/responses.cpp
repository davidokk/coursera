#include "responses.h"

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty())
        os << "No stop";
    else
        for (auto& item : r.buses)
            os << item << " ";
    return os;
}

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