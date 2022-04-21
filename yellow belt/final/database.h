#pragma once
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include "date.h"

class Database {
private:
    map<Date, pair<vector<string>, set<string>>> events;
public:
    void Add(const Date& date, const string& event);
    void Print(ostream& os) const;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    template<typename Func>
    int RemoveIf(Func predicate) {
        int ans = 0;
        for (auto& [date, record] : events) {
            set <string>& order = record.second;
            vector <string>& ev = record.first;

            Date dt = date;
            auto brd = stable_partition(ev.begin(), ev.end(), [predicate, dt](const string& event){
                return !predicate(dt, event);
            });

            for (auto itr = brd; itr != ev.end(); itr++)
                order.erase(*itr);

            ans += ev.end() - brd;
            ev.erase(brd, ev.end());
        }


        vector <Date> to_delete;
        for (auto & event : events)
            if (event.second.first.empty())
                to_delete.push_back(event.first);

        for (auto date : to_delete)
            events.erase(date);

        return ans;
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    template<typename Func>
    vector <string> FindIf(Func predicate) const {
        vector <string> ans;
        for (auto& [date, record] : events) {
            const set <string>& order = record.second;
            const vector <string>& ev = record.first;
            for (auto& cur_ev : ev) {
                if (predicate(date, cur_ev)) {
                    ostringstream os;
                    os << date << " " << cur_ev;
                    ans.push_back(os.str());
                }
            }
        }
        return ans;
    }

    string Last(const Date& date) const;
};