#include <iostream>
#include <set>
#include <vector>
using namespace std;

template <typename T>
vector <T> FindGreaterElements(const set<T>& elements, const T& border) {
    vector <T> v;
    for (auto itr = elements.begin(); itr != elements.end(); ++itr)
        if (*itr > border)
            v.push_back(*itr);
    return v;
}

int main() {
    for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
        cout << x << " ";
    }
    cout << endl;

    string to_find = "Python";
    cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
    return 0;
}

