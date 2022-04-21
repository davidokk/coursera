#pragma once

#include "Utility.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <set>

enum class RouteType {
  CYCLE, ROUND_TRIP
};

std::ostream& operator<<(std::ostream& os, RouteType route_type);

class Stop;
using StopPtr = std::shared_ptr<Stop>;

class Bus;
using BusPtr = std::shared_ptr<Bus>;

class Stop {
 public:
  explicit Stop(std::string name);

  void SetCoords(const Coordinates& new_coords);
  void AddDistToAnotherStop(const StopPtr&, int);
  void AddBus(const BusPtr&);

  std::string_view GetName() const;
  const Coordinates& GetCoords() const;
  const std::set<std::string>& GetBusesList() const;

  double ComputeGeographicalDistanceTo(const StopPtr& stop_to);
  int ComputeRealDistanceTo(const StopPtr& stop_to);

 private:
  std::string name;
  Coordinates coords = Coordinates{0, 0};
  std::unordered_map<std::string_view, int> another_stops_dist;
  std::set<std::string> buses;
};
