#include "database.h"
#include <algorithm>
#include <sstream>

void Database::Add(const Date &date, const string &event) {
    if (events.find(date) != events.end()) {
        set <string>& order = events.at(date).second;
        vector <string>& ev = events.at(date).first;
        if (order.find(event) == order.end()) {
            ev.push_back(event);
            order.insert(event);
        }
    }
    else {
        events[date].first.push_back(event);
        events[date].second.insert(event);
    }
}

void Database::Print(ostream &os) const {
    for (const auto& [date, record] : events) {
        const vector <string>& ev = record.first;
        for (const auto& cur : ev)
            os << date << " " << cur << endl;
    }
}

string Database::Last(const Date &date) const {
    auto itr = events.upper_bound(date);
    if (itr == events.begin())
        return "No entries";
    else {
        itr--;
        ostringstream os;
        os << itr->first << " " << itr->second.first.back();
        return os.str();
    }
}