#include <algorithm>
#include <sstream>
#include <iomanip>

#include "Response.h"

using namespace std;

std::string StopStat::ToString() const {
  std::string result = "Stop " + name + ": ";
  if (!found)
    return result + "not found";
  if (buses.empty())
    return result + "no buses";
  result += "buses ";
  for (const auto& bus : buses)
    result += string(bus) + " ";
  return result;
}

string BusStat::ToString() const {
  stringstream result;
  result << "Bus " << number << ": ";
  if (!found) {
    result << "not found";
    return result.str();
  }
  result << stops_on_route << " stops on route, ";
  result << unique_stops << " unique stops, ";
  result << real_route_length << " route length, ";
  result << setprecision(6) << curvature << " curvature";
  return result.str();
}

Json::Node StopStat::ToJson() const {
  using Json::Node;
  map<string, Node> map;
  map["request_id"] = Node(id);
  if (found) {
    vector<Node> v;
    for (const auto& bus : buses)
      v.emplace_back(bus);
    map["buses"] = Node(v);
  } else {
    map["error_message"] = Node("not found"s);
  }
  return Node(map);
}

Json::Node BusStat::ToJson() const {
  using Json::Node;
  map<string, Node> map;
  map["request_id"] = Node(id);
  if (found) {
    map["route_length"] = Node(real_route_length);
    map["curvature"] = Node(curvature);
    map["stop_count"] = Node(stops_on_route);
    map["unique_stop_count"] = Node(unique_stops);
  } else {
    map["error_message"] = Node("not found"s);
  }
  return Node(map);
}

void PrintResponsesInTxt(const vector<ResponsePtr>& responses, ostream& output) {
  for (const auto& response : responses)
    if (response)
      output << response->ToString() << endl;
}

void PrintResponsesInJson(const vector<ResponsePtr>& responses, ostream& output) {
  vector<Json::Node> result;
  for (const auto& response : responses)
    if (response)
      result.push_back(response->ToJson());
  output << Json::Node(result).ToString();
}

std::string RouteStat::ToString() const {
  return json_node.ToString();
}

Json::Node RouteStat::ToJson() const {
  return json_node;
}

