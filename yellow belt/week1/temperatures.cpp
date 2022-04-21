#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

int main() {
    int n;
    cin >> n;

    int64_t sum = 0;
    vector <int> temperatures(n);
    for (auto& i : temperatures) {
        cin >> i;
        sum += i;
    }

    int average = sum / static_cast<int>(temperatures.size());

    vector <int> ans;
    for (int i = 0; i < n; i++)
        if (temperatures[i] > average)
            ans.push_back(i);

    cout << ans.size() << endl;
    for (auto i : ans)
        cout << i << " ";

    return 0;
}
