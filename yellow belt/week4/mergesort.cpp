#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    int n = range_end - range_begin;
    if (n < 2)
        return;

    vector <typename RandomIt::value_type> v(range_begin, range_end);
    RandomIt it = v.begin() + n/2;

    MergeSort(v.begin(), it);
    MergeSort(it, v.end());

    vector <typename RandomIt::value_type> temp;
    merge(v.begin(), it, it, v.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}


