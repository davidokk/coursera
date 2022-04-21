#include "TransportManager.h"

#include <algorithm>
#include <optional>

using namespace std;

optional<const StopPtr> TransportManager::GetStop(string_view stop_name) const {
  auto it = stops.find(stop_name);
  if (it == stops.end())
    return nullopt;
  return it->second;
}

StopPtr TransportManager::AddStop(std::string_view stop_name) {
  auto opt = GetStop(stop_name);
  if (!opt.has_value()) {
    auto ptr = make_shared<Stop>(string(stop_name));
    stops[ptr->GetName()] = ptr;
    return ptr;
  }
  return opt.value();
}

optional<const BusPtr> TransportManager::GetBus(string_view bus_number) const {
  auto it = buses.find(bus_number);
  if (it == buses.end())
    return nullopt;
  return it->second;
}

BusPtr TransportManager::AddBus(std::string_view bus_number) {
  auto opt = GetBus(bus_number);
  if (!opt.has_value()) {
    auto ptr = make_shared<Bus>(string(bus_number));
    buses[ptr->GetNumber()] = ptr;
    return ptr;
  }
  return opt.value();
}

unique_ptr<BusStat> TransportManager::GetBusStat(std::string_view number) const {
  auto stat = make_unique<BusStat>();
  stat->number = string(number);
  auto bus_opt = GetBus(number);
  if (!bus_opt.has_value()) {
    stat->found = false;
    return stat;
  }
  auto bus_ptr = bus_opt.value();
  stat->found = true;
  stat->stops_on_route = bus_ptr->GetStopsCount();
  stat->unique_stops = bus_ptr->GetUniqueStopsCount();
  stat->real_route_length = ComputeRouteRealLength(bus_ptr->GetRoute(), bus_ptr->GetRouteType());
  stat->curvature = stat->real_route_length / ComputeRouteGeographicalLength(bus_ptr->GetRoute(), bus_ptr->GetRouteType());
  return stat;
}

unique_ptr<StopStat> TransportManager::GetStopStat(std::string_view name) const {
  auto stat = make_unique<StopStat>();
  stat->name = string(name);
  auto stop_opt = GetStop(name);
  if (!stop_opt.has_value()) {
    stat->found = false;
    return stat;
  }
  auto stop_ptr = stop_opt.value();
  stat->found = true;
  stat->buses = stop_ptr->GetBusesList();
  return stat;
}

double TransportManager::ComputeRouteGeographicalLength(const Route& route, RouteType type) {
  double length = 0;
  for (int i = 0; i < route.size() - 1; ++i)
    length += route[i]->ComputeGeographicalDistanceTo(route[i + 1]);
  if (type == RouteType::ROUND_TRIP)
    length *= 2;
  return length;
}

int TransportManager::ComputeRouteRealLength(const Route& route, RouteType type) {
  int length = 0;
  for (int i = 0; i < route.size() - 1; ++i) {
    length += route[i]->ComputeRealDistanceTo(route[i + 1]);
    if (type == RouteType::ROUND_TRIP)
      length += route[i + 1]->ComputeRealDistanceTo(route[i]);
  }
  return length;
}

optional<pair<double, vector<Json::Node>>> TransportManager::GetRoute(std::string_view l, std::string_view r) {
  if (router == nullptr)
    BuildGraph();
  map<string, Json::Node> map;
  auto route_info = router->BuildRoute(stop_vertex_number[l], stop_vertex_number[r]);
  if (route_info.has_value()) {
    vector<Json::Node> route;
    route.reserve(route_info->edge_count);
    for (int i = 0; i < route_info->edge_count; ++i) {
      auto it = edges_info.find(router->GetRouteEdge(route_info->id, i));
      if (it != edges_info.end())
        route.push_back(it->second);
    }
    return make_pair(route_info->weight, route);
  }
  return nullopt;
}

Json::Node TransportManager::BuildWaitEdge(std::string_view stop) {
  std::map<std::string, Json::Node> map;
  map["type"] = "Wait"s;
  map["stop_name"] = std::string(stop);
  map["time"] = bus_wait_time;
  return Json::Node(map);
}

Json::Node TransportManager::BuildBusEdge(std::string_view name, int stop_count, double time) {
  std::map<std::string, Json::Node> map;
  map["type"] = "Bus"s;
  map["bus"] = string(name);
  map["span_count"] = stop_count;
  map["time"] = time;
  return Json::Node(map);
}

void TransportManager::SetBusVelocity(double i) { bus_velocity = i / 0.06; }
void TransportManager::SetBusWaitTime(int i) { bus_wait_time = i; }

void TransportManager::BuildGraph() {
  graph = Graph::DirectedWeightedGraph<double>(2 * stops.size());
  for (auto[stop_name, stop_ptr] : stops)
    stop_vertex_number[stop_name] = 2 * stop_vertex_number.size();
  for (auto[stop_name, stop_ptr] : stops) {
    size_t num = stop_vertex_number[stop_name];
    auto edge_id = graph->AddEdge({num, num + 1, bus_wait_time});
    graph->AddEdge({num + 1, num, 0});
    edges_info[edge_id] = BuildWaitEdge(stop_name);
  }
  for (auto[bus_name, bus_ptr] : buses) {
    Route route = bus_ptr->GetRoute();
    if (bus_ptr->GetRouteType() == RouteType::ROUND_TRIP) {
      for (int i = (int)route.size() - 2; i >= 0; --i)
        route.push_back(route[i]);
    }
    for (int i = 0; i < route.size(); ++i) {
      double length = 0;
      for (int j = i + 1; j < route.size(); ++j) {
        length += route[j - 1]->ComputeRealDistanceTo(route[j]);
        size_t l = stop_vertex_number[route[i]->GetName()];
        size_t r = stop_vertex_number[route[j]->GetName()];
        auto edge_id = graph->AddEdge({l + 1, r, length / bus_velocity});
        edges_info[edge_id] = BuildBusEdge(bus_name, j - i, length / bus_velocity);
      }
    }
  }
  router = std::make_unique<Graph::Router<double>>(graph.value());
}