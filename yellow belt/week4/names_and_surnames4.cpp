#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
        name[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        surname[year] = last_name;
    }
    string GetFullName(int year) {
        string cur_name = "none", cur_surname = "none";

        auto itr_name = name.upper_bound(year);
        if (itr_name != name.begin())
            cur_name = (--itr_name)->second;

        auto itr_surname = surname.upper_bound(year);
        if (itr_surname != surname.begin())
            cur_surname = (--itr_surname)->second;

        if (cur_name == "none" && cur_surname == "none")
            return "Incognito";
        if (cur_name == "none")
            return cur_surname + " with unknown first name";
        if (cur_surname == "none")
            return cur_name + " with unknown last name";
        return cur_name + " " + cur_surname;
    }
private:
    map <int, string> name;
    map <int, string> surname;
};

int main() {
    Person person;

    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : {1900, 1965, 1990}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeLastName(1968, "Volkova");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    return 0;
}
