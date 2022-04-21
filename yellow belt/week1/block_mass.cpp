#include <iostream>
#include <cstdint>
using namespace std;

int main() {
    uint64_t n, r;
    cin >> n >> r;

    uint64_t sum = 0;
    for (int i = 0; i < n; ++i) {
        uint64_t w, h, d;
        cin >> w >> h >> d;
        sum += r * w * h * d;
    }

    cout << sum;
    return 0;
}
