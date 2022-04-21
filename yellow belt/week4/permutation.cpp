#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
using namespace std;

void print(const vector<int>& v) {
    for (auto& i : v)
        cout << i << " ";
    cout << endl;
}

int main() {

    int n;
    cin >> n;
    vector <int> v(n);
    iota(v.rbegin(), v.rend(), 1);

    do {
        print(v);
    } while (prev_permutation(v.begin(), v.end()));

    return 0;
}
