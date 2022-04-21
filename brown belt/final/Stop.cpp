#include "Stop.h"
#include "Bus.h"

using namespace std;

Stop::Stop(string name_) : name(move(name_)) {}

void Stop::SetCoords(const Coordinates& new_coords) { coords = new_coords; }

void Stop::AddDistToAnotherStop(const StopPtr& stop, int dist) {
  another_stops_dist[stop->GetName()] = dist;
}

void Stop::AddBus(const BusPtr& bus) {
  buses.insert(string(bus->GetNumber()));
}

const std::set<std::string>& Stop::GetBusesList() const { return buses; }

string_view Stop::GetName() const { return name; }
const Coordinates& Stop::GetCoords() const { return coords; }

std::ostream& operator<<(std::ostream& os, RouteType route_type) {
  if (route_type == RouteType::CYCLE)
    return os << "CYCLE";
  else
    return os << "ROUND_TRIP";
}

double Stop::ComputeGeographicalDistanceTo(const StopPtr& stop_to) {
  return ComputeDistance(coords, stop_to->GetCoords());
}

int Stop::ComputeRealDistanceTo(const StopPtr& stop_to) {
  if (another_stops_dist.count(stop_to->GetName()) == 0)
    return stop_to->ComputeRealDistanceTo(make_shared<Stop>(name));
  return another_stops_dist[stop_to->GetName()];
}
