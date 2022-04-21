#include <iostream>
#include <algorithm>
#include "TestRunner.h"
using namespace std;

bool IsPalindrom(const string& s) {
    string t = s;
    reverse(begin(t), end(t));
    return t == s;
}

void TestPalindrom() {
    Assert(IsPalindrom(""), "1");
    Assert(IsPalindrom("a"), "2");
    Assert(IsPalindrom("aaa"), "3");
    Assert(IsPalindrom("abcba"), "4");
    Assert(IsPalindrom("abccba"), "5");
    Assert(!IsPalindrom("ab"), "6");
    Assert(!IsPalindrom("aaab"), "7");
    Assert(!IsPalindrom("baaa"), "8");
    Assert(!IsPalindrom("baaac"), "9");
    Assert(!IsPalindrom("kfioewjog"), "10");
    Assert(IsPalindrom("wasitacaroracatisaw"), "11");
    Assert(IsPalindrom("madam"), "12");
    Assert(IsPalindrom("level"), "13");
    Assert(IsPalindrom("a a a"), "14");
    Assert(!IsPalindrom(" aaa"), "15");
    Assert(!IsPalindrom("aaa "), "16");
    Assert(IsPalindrom(" aaa "), "17");
    Assert(IsPalindrom(" aaaa "), "18");
    Assert(IsPalindrom(" a a  a a "), " 19");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestPalindrom, "TestPalindrom");
}

int main() {
    TestAll();
    return 0;
}
