#include <iostream>
#include <sstream>
#include "phone_number.h"

void check(string s) {
    if (s.empty())
        throw invalid_argument("invalid_argument");
}

PhoneNumber::PhoneNumber(const string &international_number) {
    stringstream ss(international_number);
    if (ss.peek() != '+')
        throw invalid_argument("");
    ss.ignore(1);
    getline(ss, country_code_, '-');
    check(country_code_);
    getline(ss, city_code_, '-');
    check(city_code_);
    ss >> local_number_;
    check(local_number_);
}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    string s = "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
    return s;
}

int main() {
    try {
        PhoneNumber number("+375-29-8066676");
        cout << number.GetInternationalNumber() << endl;
        cout << number.GetLocalNumber() << endl;
        cout << number.GetCountryCode() << endl;
        cout << number.GetCityCode() << endl;
    }
    catch (exception& ex) {
        cout << ex.what();
    }
    return 0;
}
