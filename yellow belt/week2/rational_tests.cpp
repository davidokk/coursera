#include <iostream>
#include "TestRunner.h"

int GCD(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

class Rational {
private:
    int num;
    int denom;
    void Normalize() {
        if (denom < 0) {
            num = -num;
            denom = -denom;
        }
        int gcd = GCD(abs(num), denom);
        num /= gcd;
        denom /= gcd;
    }
public:
    Rational() {
        num = 0;
        denom = 1;
    }
    Rational(int p, int q) {
        num = p;
        denom = q;
        if (num == 0)
            denom = 1;
        Normalize();
    }

    int Numerator() const {
        return num;
    }

    int Denominator() const {
        return denom;
    }
};

void TestRational() {
    {
        Rational a;
        AssertEqual(a.Numerator(), 0, "1");
        AssertEqual(a.Denominator(), 1, "2");
    }
    {
        Rational a(2, 4);
        AssertEqual(a.Numerator(), 1, "3");
        AssertEqual(a.Denominator(), 2, "4");
    }
    {
        Rational a(-2, -2);
        AssertEqual(a.Numerator(), 1, "5");
        AssertEqual(a.Denominator(), 1, "6");
    }
    {
        Rational a(-45, 5);
        AssertEqual(a.Numerator(), -9, "7");
        AssertEqual(a.Denominator(), 1, "8");
    }
    {
        Rational a(45, -5);
        AssertEqual(a.Numerator(), -9, "7");
        AssertEqual(a.Denominator(), 1, "8");
    }
    {
        Rational a(0, 123);
        AssertEqual(a.Numerator(), 0, "9");
        AssertEqual(a.Denominator(), 1, "10");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestRational, "TestRational");
}

int main() {
    TestAll();
    return 0;
}
