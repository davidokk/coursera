#include "Request.h"
#include "TransportManager.h"

using namespace std;

void BaseStopRequest::ParseFromString(std::string_view input) {
  name = string(DeleteSpaces(ReadToken(input, ":")));
  coords = Coordinates{
      ConvertTo<double>(ReadToken(input, ",")),
      ConvertTo<double>(ReadToken(input, ","))
  };
  string_view dist_to_another_stop;
  while (!(dist_to_another_stop = ReadToken(input, ",")).empty()) {
    auto[dist_str, stop_name] = SplitTwo(dist_to_another_stop, "m to ");
    road_distances[string(stop_name)] = ConvertTo<int>(dist_str);
  }
}

ResponsePtr BaseStopRequest::Process(TransportManager& manager) const {
  StopPtr stop_ptr = manager.AddStop(name);
  stop_ptr->SetCoords(coords);
  for (auto[stop_to, dist] : road_distances) {
    StopPtr stop_to_ptr = manager.AddStop(stop_to);
    stop_ptr->AddDistToAnotherStop(stop_to_ptr, dist);
  }
  return nullptr;
}

void StopStatRequest::ParseFromString(std::string_view input) {
  name = string(input);
}

ResponsePtr StopStatRequest::Process(TransportManager& manager) const {
  auto stat = manager.GetStopStat(name);
  stat->id = id;
  return stat;
}

void BaseBusRequest::ParseFromString(std::string_view input) {
  number = ReadToken(input, ":");
  string_view delimiter;
  if (input.find('>') != std::string_view::npos) {
    delimiter = ">";
    route_type = RouteType::CYCLE;
  } else {
    delimiter = "-";
    route_type = RouteType::ROUND_TRIP;
  }
  string_view stop_name;
  while (!(stop_name = ReadToken(input, delimiter)).empty())
    route.emplace_back(DeleteSpaces(stop_name));
}

ResponsePtr BaseBusRequest::Process(TransportManager& manager) const {
  BusPtr bus = manager.AddBus(number);
  bus->SetRouteType(route_type);
  for (auto& stop : route) {
    bus->AddStop(manager.AddStop(stop));
    manager.AddStop(stop)->AddBus(bus);
  }
  return nullptr;
}

void BusStatRequest::ParseFromString(std::string_view input) {
  number = string(input);
}

ResponsePtr BusStatRequest::Process(TransportManager& manager) const {
  auto stat = manager.GetBusStat(number);
  stat->id = id;
  return stat;
}

std::vector<RequestHolder> ReadRequestsFromTxt(std::istream& input) {
  std::vector<RequestHolder> result;
  int base_request_count = ReadNumberOnLine<int>(input);
  result.reserve(base_request_count);

  string line;
  while (base_request_count--) {
    getline(input, line);
    auto[type, query] = SplitTwo(line, " ");
    if (type == "Bus")
      result.push_back(make_unique<BaseBusRequest>());
    else
      result.push_back(make_unique<BaseStopRequest>());
    result.back()->ParseFromString(query);
  }

  int stat_request_count = ReadNumberOnLine<int>(input);
  while (stat_request_count--) {
    getline(input, line);
    auto[type, query] = SplitTwo(line, " ");
    if (type == "Bus")
      result.push_back(make_unique<BusStatRequest>());
    else
      result.push_back(make_unique<StopStatRequest>());
    result.back()->ParseFromString(query);
  }
  return result;
}

std::vector<ResponsePtr> ProcessRequests(const vector<RequestHolder>& requests) {
  TransportManager manager;
  std::vector<ResponsePtr> result;
  result.reserve(requests.size());
  for (const auto& request : requests)
    result.push_back(request->Process(manager));
  return result;
}

void BaseStopRequest::ParseFromJson(Json::Node node) {
  const auto& map = node.AsMap();
  name = map.at("name").AsString();
  coords = Coordinates{
    map.at("latitude").AsDouble(),
    map.at("longitude").AsDouble(),
  };
  for (auto[stop_to, dist] : map.at("road_distances").AsMap())
      road_distances[stop_to] = dist.AsInt();
}

void StopStatRequest::ParseFromJson(Json::Node node) {
  const auto& map = node.AsMap();
  name = map.at("name").AsString();
  id = map.at("id").AsInt();
}

void BaseBusRequest::ParseFromJson(Json::Node node) {
  const auto& map = node.AsMap();
  number = map.at("name").AsString();
  route_type = map.at("is_roundtrip").AsBool() ? RouteType::CYCLE : RouteType::ROUND_TRIP;
  for (const auto& stop : map.at("stops").AsArray())
    route.push_back(stop.AsString());
}

void BusStatRequest::ParseFromJson(Json::Node node) {
  const auto& map = node.AsMap();
  number = map.at("name").AsString();
  id = map.at("id").AsInt();
}

std::vector<RequestHolder> ReadRequestsFromJson(std::istream& input) {
  std::vector<RequestHolder> result;
  Json::Node root = Json::Load(input).GetRoot();
  for (const auto& node : root.AsMap().at("base_requests").AsArray()) {
    if (node.AsMap().at("type").AsString() == "Stop")
      result.push_back(make_unique<BaseStopRequest>());
    else
      result.push_back(make_unique<BaseBusRequest>());
    result.back()->ParseFromJson(node);
  }
  result.push_back(make_unique<BaseRoutingSettingsRequest>());
  result.back()->ParseFromJson(root.AsMap().at("routing_settings"));
  for (const auto& node : root.AsMap().at("stat_requests").AsArray()) {
    if (node.AsMap().at("type").AsString() == "Stop")
      result.push_back(make_unique<StopStatRequest>());
    else if (node.AsMap().at("type").AsString() == "Bus")
      result.push_back(make_unique<BusStatRequest>());
    else
      result.push_back(make_unique<RouteStatRequest>());
    result.back()->ParseFromJson(node);
  }
  return result;
}

void BaseRoutingSettingsRequest::ParseFromJson(Json::Node node) {
  bus_velocity = node.AsMap().at("bus_velocity").AsDouble();
  bus_wait_time = node.AsMap().at("bus_wait_time").AsInt();
}

ResponsePtr BaseRoutingSettingsRequest::Process(TransportManager& manager) const {
  manager.SetBusVelocity(bus_velocity);
  manager.SetBusWaitTime(bus_wait_time);
  return nullptr;
}

void RouteStatRequest::ParseFromJson(Json::Node node) {
  from = node.AsMap().at("from").AsString();
  to = node.AsMap().at("to").AsString();
  id = node.AsMap().at("id").AsInt();
}

ResponsePtr RouteStatRequest::Process(TransportManager& manager) const {
  auto stat = make_unique<RouteStat>();
  auto route_opt = manager.GetRoute(from, to);
  map<string, Json::Node> map;
  map["request_id"] = id;
  if (!route_opt.has_value()) {
    map["error_message"] = "not found"s;
  } else {
    map["total_time"] = route_opt->first;
    map["items"] = route_opt->second;
  }
  stat->json_node = map;
  return stat;
}
