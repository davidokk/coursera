#pragma once

#include "Bus.h"
#include "Json.h"
#include "Request.h"
#include "Router.h"

#include <iostream>
#include <unordered_map>

class TransportManager {
 public:
  std::optional<const StopPtr> GetStop(std::string_view) const;
  std::optional<const BusPtr> GetBus(std::string_view) const;

  StopPtr AddStop(std::string_view);
  BusPtr AddBus(std::string_view);

  static double ComputeRouteGeographicalLength(const Route& route, RouteType type) ;
  static int ComputeRouteRealLength(const Route& route, RouteType type) ;

  std::unique_ptr<BusStat> GetBusStat(std::string_view) const;
  std::unique_ptr<StopStat> GetStopStat(std::string_view) const;

  std::optional<std::pair<double, std::vector<Json::Node>>> GetRoute(std::string_view, std::string_view);

  void SetBusVelocity(double);
  void SetBusWaitTime(int);

 private:
  std::unordered_map<std::string_view, BusPtr> buses;
  std::unordered_map<std::string_view, StopPtr> stops;

  double bus_velocity;
  double bus_wait_time;

  std::optional<Graph::DirectedWeightedGraph<double>> graph;
  std::unique_ptr<Graph::Router<double>> router;
  std::unordered_map<std::string_view, int> stop_vertex_number;
  std::unordered_map<int, Json::Node> edges_info;

  void BuildGraph();

  Json::Node BuildWaitEdge(std::string_view stop);
  Json::Node BuildBusEdge(std::string_view name, int stop_count, double time);

};

