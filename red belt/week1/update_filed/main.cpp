#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

istream& operator>>(istream& is, Date& d) {
  is >> d.year;
  is.ignore(1);
  is >> d.month;
  is.ignore(1);
  return is >> d.day;
}

istream& operator>>(istream& is, Time& t) {
  is >> t.hours;
  is.ignore(1);
  return is >> t.minutes;
}

#define UPDATE_FIELD(ticket, field, values)                              \
  for (auto[cur_field, new_value] : values) {                            \
    if (cur_field == #field) {                                           \
      istringstream is(new_value);                                       \
      is >> ticket.field;                                                \
    }                                                                    \
  }

bool operator==(const Time& l, const Time& r) {
  return make_tuple(l.hours, l.minutes) == make_tuple(r.hours, r.minutes);
}
bool operator==(const Date& l, const Date& r) {
  return make_tuple(l.year, l.month, l.day) == make_tuple(r.year, r.month, r.day);
}
ostream& operator<< (ostream& os, const Time& r) {
  return os << r.hours << ":" << r.minutes;
}
ostream& operator<< (ostream& os, const Date& r) {
  return os << r.year << "." << r.month << "." << r.day;
}


void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
      {"departure_date", "2018-2-28"},
      {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
      {"price", "12550"},
      {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}