#pragma once

#include "Response.h"
#include "Json.h"
#include "Utility.h"

class TransportManager;

struct Request {
  virtual ResponsePtr Process(TransportManager&) const = 0;
  virtual void ParseFromString(std::string_view) {}
  virtual void ParseFromJson(Json::Node node) {}

  virtual ~Request() = default;
};

struct BaseStopRequest : Request {
  void ParseFromString(std::string_view) override;
  void ParseFromJson(Json::Node node) override;

  ResponsePtr Process(TransportManager&) const override;

  std::string name;
  Coordinates coords;
  std::map<std::string, int> road_distances;
};

struct StopStatRequest : Request {
  void ParseFromString(std::string_view) override;
  void ParseFromJson(Json::Node node) override;

  ResponsePtr Process(TransportManager&) const override;

  std::string name;
  int id;
};

struct BaseBusRequest : Request {
  void ParseFromString(std::string_view) override;
  void ParseFromJson(Json::Node node) override;

  ResponsePtr Process(TransportManager&) const override;

  std::string number;
  std::vector<std::string> route;
  RouteType route_type;
};

struct BusStatRequest : Request {
  void ParseFromString(std::string_view) override;
  void ParseFromJson(Json::Node node) override;

  ResponsePtr Process(TransportManager&) const override;

  std::string number;
  int id;
};

struct BaseRoutingSettingsRequest : Request {
  void ParseFromJson(Json::Node node) override;

  ResponsePtr Process(TransportManager&) const override;

  double bus_velocity;
  int bus_wait_time;
};

struct RouteStatRequest : Request {
  void ParseFromJson(Json::Node node) override;

  ResponsePtr Process(TransportManager&) const override;

  std::string from;
  std::string to;
  int id;
};

using RequestHolder = std::unique_ptr<Request>;

std::vector<RequestHolder> ReadRequestsFromTxt(std::istream&);
std::vector<RequestHolder> ReadRequestsFromJson(std::istream&);

std::vector<ResponsePtr> ProcessRequests(const std::vector<RequestHolder>& requests);
