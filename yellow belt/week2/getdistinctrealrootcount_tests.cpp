#include <iostream>
#include "TestRunner.h"

using namespace std;

int GetDistinctRealRootCount(double a, double b, double c) {
    if (a != 0) {
        double d = b * b - 4 * a * c;
        if (d < 0)
            return 0;
        if (d == 0)
            return 1;
        return 2;
    }
    else {
        if (b == 0)
            return 0;
        return 1;
    }
}

void TestQuadratic() {
    AssertEqual(GetDistinctRealRootCount(1, 2, 3), 0, "1");
    AssertEqual(GetDistinctRealRootCount(1, 5, 6), 2, "2");
    AssertEqual(GetDistinctRealRootCount(1, 2, 1), 1, "3");
    AssertEqual(GetDistinctRealRootCount(2, 0, 0), 1, "4");
    AssertEqual(GetDistinctRealRootCount(2, 0, 1), 0, "5");
    AssertEqual(GetDistinctRealRootCount(2, 0, -1), 2, "6");
    AssertEqual(GetDistinctRealRootCount(1, 1, 0), 2, "7");
}

void TestLinear() {
    AssertEqual(GetDistinctRealRootCount(0, 1, 0), 1, "1");
    AssertEqual(GetDistinctRealRootCount(0, 0, 1), 0, "2");
    AssertEqual(GetDistinctRealRootCount(0, 1, 1), 1, "3");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestQuadratic, "TestQuadratic");
    tr.RunTest(TestLinear, "TestLinear");
}

int main() {
    TestAll();
}