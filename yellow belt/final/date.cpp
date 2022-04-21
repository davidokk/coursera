#include "date.h"
#include <tuple>
#include <iomanip>
using namespace std;

Date::Date(int new_year, int new_moth, int new_day) {
    year = new_year;
    month = new_moth;
    day = new_day;
}

bool Date::operator<(const Date &r) const {
    return make_tuple(year, month, day) < make_tuple(r.year, r.month, r.day);
}
bool Date::operator>(const Date &r) const {
    return make_tuple(year, month, day) > make_tuple(r.year, r.month, r.day);
}
bool Date::operator<=(const Date &r) const {
    return make_tuple(year, month, day) <= make_tuple(r.year, r.month, r.day);
}
bool Date::operator>=(const Date &r) const {
    return make_tuple(year, month, day) >= make_tuple(r.year, r.month, r.day);
}
bool Date::operator==(const Date &r) const {
    return make_tuple(year, month, day) == make_tuple(r.year, r.month, r.day);
}
bool Date::operator!=(const Date &r) const {
    return make_tuple(year, month, day) != make_tuple(r.year, r.month, r.day);
}

ostream& operator<< (ostream& os, const Date& date) {
    return os << setw(4) << setfill('0') << date.year << '-' <<
    setw(2) << setfill('0') << date.month << '-' <<
    setw(2) << setfill('0') << date.day;
}

Date ParseDate(istream& is) {
    int year, month, day;
    is >> year;
    is.ignore(1);
    is >> month;
    is.ignore(1);
    is >> day;
    return Date(year, month, day);
}