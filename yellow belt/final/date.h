#pragma once

#include <iostream>
using namespace std;

class Date {
private:
    int day;
    int month;
    int year;
public:
    explicit Date(int new_year, int new_moth, int new_day);
    bool operator< (const Date& r) const;
    bool operator> (const Date& r) const;
    bool operator<= (const Date& r) const;
    bool operator>= (const Date& r) const;
    bool operator== (const Date& r) const;
    bool operator!= (const Date& r) const;
    friend ostream& operator<< (ostream& os, const Date& date);
};

Date ParseDate(istream& is);


