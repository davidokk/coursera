#include <iostream>
#include <tuple>
#include <map>
#include <vector>
using namespace std;

enum class Lang {
    DE, FR, IT
};

struct Region {
    string std_name;
    string parent_std_name;
    map<Lang, string> names;
    int64_t population;
};

bool operator< (const Region& l, const Region& r) {
    auto lt = tie(l.std_name, l.parent_std_name, l.names, l.population);
    auto rt = tie(r.std_name, r.parent_std_name, r.names, r.population);
    return lt < rt;
}

int FindMaxRepetitionCount(const vector<Region>& regions) {
    map <Region, int> regions_count;
    for (auto& region : regions)
        regions_count[region]++;

    int ans = 0;
    for (auto& [region, count] : regions_count)
        ans = max(ans, count);

    return ans;
}

int main() {

}