#include <iostream>
#include <vector>
#include <map>
#include <utility>
using namespace std;

template <typename First, typename Second> pair <First, Second> Sqr(pair <First, Second> p);
template <typename T> vector <T> Sqr(vector <T> v);
template <typename Key, typename Value> map<Key, Value> Sqr(map <Key, Value> m);

template <typename T>
T Sqr(T a) {
    return a * a;
}

template <typename First, typename Second>
pair <First, Second> Sqr(pair <First, Second> p) {
    return make_pair(Sqr(p.first), Sqr(p.second));
}

template <typename T>
vector <T> Sqr(vector <T> v) {
    for (auto& i : v)
        i = Sqr(i);
    return v;
}

template <typename Key, typename Value>
map <Key, Value> Sqr(map <Key, Value> m) {
    for (auto& [key, value] : m)
        value = Sqr(value);
    return m;
}

int main() {

}
