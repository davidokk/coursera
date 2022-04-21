#pragma once

#include "Json.h"
#include "Bus.h"

#include <iostream>
#include <memory>

struct Response {
  virtual std::string ToString() const = 0;
  virtual Json::Node ToJson() const = 0;

  virtual ~Response() = default;
};

// todo StopStatBuilder
struct StopStat : Response {
  std::string ToString() const override;
  Json::Node ToJson() const override;

  int id;
  std::string name;
  bool found;
  std::set<std::string> buses;
};

// todo BusStatBuilder
struct BusStat : Response {
  std::string ToString() const override;
  Json::Node ToJson() const override;

  int id;
  std::string number;
  bool found;
  int stops_on_route;
  int unique_stops;
  int real_route_length;
  double curvature;
};

struct RouteStat : Response {
  std::string ToString() const override;
  Json::Node ToJson() const override;

  Json::Node json_node;
};

using ResponsePtr = std::unique_ptr<Response>;

void PrintResponsesInTxt(const std::vector<ResponsePtr>& responses, std::ostream& output);
void PrintResponsesInJson(const std::vector<ResponsePtr>& responses, std::ostream& output);