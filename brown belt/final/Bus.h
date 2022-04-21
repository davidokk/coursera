#pragma once

#include "Stop.h"

#include <vector>
#include <memory>
#include <unordered_set>

using Route = std::vector<StopPtr>;

class Bus {
 public:
  explicit Bus(std::string number_);

  void SetRouteType(RouteType type);
  void AddStop(const StopPtr& stop);

  std::string_view GetNumber() const;
  const Route& GetRoute() const;
  RouteType GetRouteType() const;

  int GetStopsCount() const;
  int GetUniqueStopsCount() const;

 private:
  std::string number;
  Route route;
  RouteType route_type = RouteType::CYCLE;
  std::unordered_set<std::string_view> unique_stops;
};

using BusPtr = std::shared_ptr<Bus>;
