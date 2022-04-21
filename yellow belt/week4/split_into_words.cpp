#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector <string> SplitIntoWords(const string& s) {
    vector <string> ans;

    auto previous = s.begin();
    while (previous != s.end()) {
        auto space = find(previous, s.end(), ' ');
        ans.push_back(string(previous, space));
        previous = space;
        if (previous != s.end())
            previous++;
    }

    return ans;
}

int main() {
    string s = "C Cpp Java Python";

    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;

    return 0;
}

