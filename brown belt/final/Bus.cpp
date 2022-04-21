#include "Bus.h"

#include <utility>

using namespace std;

Bus::Bus(string number_) : number(move(number_)) {}

void Bus::SetRouteType(RouteType type) { route_type = type; }

void Bus::AddStop(const StopPtr& stop) {
  unique_stops.insert(stop->GetName());
  route.push_back(stop);
}

std::string_view Bus::GetNumber() const { return number; }
const Route& Bus::GetRoute() const { return route; }
RouteType Bus::GetRouteType() const { return route_type; }

int Bus::GetStopsCount() const {
  if (route_type == RouteType::CYCLE)
    return route.size();
  else
    return 2 * static_cast<int>(route.size()) - 1;
}

int Bus::GetUniqueStopsCount() const {
  return unique_stops.size();
}
