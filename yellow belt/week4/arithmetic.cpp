#include <iostream>
#include <deque>
using namespace std;

int main() {
    int x;
    cin >> x;

    deque <string> dq;
    dq.push_back(to_string(x));

    int n;
    cin >> n;
    // char prevoperation = '*';
    while (n--) {
        char operation;
        cin >> operation;
        string number;
        cin >> number;
        // if ((operation == '*' || operation == '/') && (prevoperation == '+' || prevoperation == '-')) {
            dq.push_front("(");
            dq.push_back(") ");
            dq.push_back(string(1, operation));
            dq.push_back(" ");
            dq.push_back(number);
        // }
        // else {
        //     dq.push_back(" ");
        //     dq.push_back(string(1, operation));
        //     dq.push_back(" ");
        //     dq.push_back(number);
        // }
        //
        // prevoperation = operation;
    }

    for (auto i : dq)
        cout << i;

    return 0;
}
