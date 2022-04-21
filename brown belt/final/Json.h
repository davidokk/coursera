#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

class Node : std::variant<std::vector<Node>,
                          std::map<std::string, Node>,
                          int,
                          std::string,
                          bool,
                          double> {
 public:
  using variant::variant;

  const auto& AsArray() const {
    return std::get<std::vector<Node>>(*this);
  }
  const auto& AsMap() const {
    return std::get<std::map<std::string, Node>>(*this);
  }
  int AsInt() const {
    return std::get<int>(*this);
  }
  const auto& AsString() const {
    return std::get<std::string>(*this);
  }
  bool AsBool() const {
    return std::get<bool>(*this);
  }
  double AsDouble() const {
    if (!std::holds_alternative<double>(*this))
      return std::get<int>(*this);
    return std::get<double>(*this);
  }

  std::string ToString() const;
};

class Document {
 public:
  explicit Document(Node root);

  const Node& GetRoot() const;

 private:
  Node root;
};

Document Load(std::istream& input);

}