#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void PrintVectorPart(const vector<int>& numbers) {
    auto itr = find_if(numbers.begin(), numbers.end(), [](const int& a){return a < 0;});
    while (itr != numbers.begin()) {
        itr--;
        cout << *itr << " ";
    }
    cout << endl;
}

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    cout << endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    cout << endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    cout << endl;
    return 0;
}

