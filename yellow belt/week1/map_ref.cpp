#include <iostream>
#include <map>
using namespace std;

template <typename Key, typename Value>
Value& GetRefStrict(map <Key, Value>& m, Key key) {
    if (m.count(key) == 0)
        throw runtime_error("");
    return m[key];
}

int main() {

    return 0;
}
