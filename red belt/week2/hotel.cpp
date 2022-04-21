#include <iostream>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <deque>

using namespace std;

class HotelManager {
 private:
  struct booking {
    int client_id;
    long long time;
    int rooms;
    //bool operator<(const booking& a) const { return time < a.time; }
  };

  struct record {
    map<int, int> clients;
    deque<booking> last_events;
    int total_clients;
    int total_rooms;
  };

 public:
  void Book(long long time, const string& hotel_name, int client_id, int room_count) {
    current_time = time;
    record& hotel = hotels[hotel_name];

    hotel.last_events.push_back({client_id, time, room_count});
    if (hotel.clients[client_id]++ == 0)
      ++hotel.total_clients;

    hotel.total_rooms += room_count;

    DeleteOldEvents(hotel);
  }

  void DeleteOldEvents(record& r) {
    while (!r.last_events.empty() && r.last_events.front().time <= current_time - 86400) {
      int client = r.last_events.front().client_id;
      int rooms = r.last_events.front().rooms;

      --r.clients[client];
      if (r.clients[client] == 0)
        --r.total_clients;

      r.total_rooms -= rooms;

      r.last_events.pop_front();
    }
  }

  int Clients(const string& hotel_name) {
    if (hotels.count(hotel_name) == 0)
      return 0;
    DeleteOldEvents(hotels[hotel_name]);
    return hotels[hotel_name].total_clients;
  }

  int Rooms(const string& hotel_name) {
    if (hotels.count(hotel_name) == 0)
      return 0;
    DeleteOldEvents(hotels[hotel_name]);
    return hotels[hotel_name].total_rooms;
  }

 private:
  map<string, record> hotels;
  long long current_time = 0;
};

int main() {

  // ifstream cin("input.txt");
  HotelManager manager;

  int q;
  cin >> q;
  while (q--) {
    string query;
    cin >> query;
    if (query == "BOOK") {
      int client_id, room;
      long long time;
      string hotel_name;
      cin >> time >> hotel_name >> client_id >> room;
      manager.Book(time, hotel_name, client_id, room);
    } else if (query == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Clients(hotel_name) << endl;
    } else {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Rooms(hotel_name) << endl;
    }
  }

  return 0;
}
