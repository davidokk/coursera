#include <iostream>
#include "TestRunner.h"
using namespace std;

string FindNameByYear(const map<int, string>& names, int year) {
    string name;  // изначально имя неизвестно

    // перебираем всю историю по возрастанию ключа словаря, то есть в хронологическом порядке
    for (const auto& item : names) {
        // если очередной год не больше данного, обновляем имя
        if (item.first <= year) {
            name = item.second;
        } else {
            // иначе пора остановиться, так как эта запись и все последующие относятся к будущему
            break;
        }
    }

    return name;
}

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
        first_names[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        last_names[year] = last_name;
    }
    string GetFullName(int year) {
        // получаем имя и фамилию по состоянию на год year
        const string first_name = FindNameByYear(first_names, year);
        const string last_name = FindNameByYear(last_names, year);

        // если и имя, и фамилия неизвестны
        if (first_name.empty() && last_name.empty()) {
            return "Incognito";

            // если неизвестно только имя
        } else if (first_name.empty()) {
            return last_name + " with unknown first name";

            // если неизвестна только фамилия
        } else if (last_name.empty()) {
            return first_name + " with unknown last name";

            // если известны и имя, и фамилия
        } else {
            return first_name + " " + last_name;
        }
    }

private:
    map<int, string> first_names;
    map<int, string> last_names;
};

void TestGetFullName() {
    {
        Person person;
        person.ChangeLastName(2000, "Gutko");
        person.ChangeFirstName(2003, "David");

        AssertEqual(person.GetFullName(1000), "Incognito", "1");
        AssertEqual(person.GetFullName(2000), "Gutko with unknown first name", "2");
        AssertEqual(person.GetFullName(2001), "Gutko with unknown first name", "3");
        AssertEqual(person.GetFullName(2003), "David Gutko", "4");
        AssertEqual(person.GetFullName(2004), "David Gutko", "5");

    }
    {
        Person person;
        person.ChangeFirstName(2000, "David");
        person.ChangeLastName(2003, "Gutko");

        AssertEqual(person.GetFullName(1000), "Incognito", "6");
        AssertEqual(person.GetFullName(2000), "David with unknown last name", "7");
        AssertEqual(person.GetFullName(2001), "David with unknown last name", "8");
        AssertEqual(person.GetFullName(2003), "David Gutko", "9");
        AssertEqual(person.GetFullName(2004), "David Gutko", "10");
    }
    {
        Person person;
        person.ChangeFirstName(2004, "David");
        person.ChangeLastName(2003, "Gutko");
        person.ChangeFirstName(2006, "Davidik");
        person.ChangeFirstName(2008, "Davidok");

        AssertEqual(person.GetFullName(2002), "Incognito", "11");
        AssertEqual(person.GetFullName(2003), "Gutko with unknown first name", "12");
        AssertEqual(person.GetFullName(2004), "David Gutko", "13");
        AssertEqual(person.GetFullName(2005), "David Gutko", "14");
        AssertEqual(person.GetFullName(2006), "Davidik Gutko", "15");
        AssertEqual(person.GetFullName(2007), "Davidik Gutko", "16");
        AssertEqual(person.GetFullName(2008), "Davidok Gutko", "17");
        AssertEqual(person.GetFullName(2009), "Davidok Gutko", "18");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestGetFullName, "TestGetFullName");
}

int main() {
    TestAll();
}