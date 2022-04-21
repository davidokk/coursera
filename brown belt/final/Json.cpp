#include <sstream>
#include <iomanip>

#include "Json.h"

using namespace std;

namespace Json {

Document::Document(Node root) : root(move(root)) {
}

const Node& Document::GetRoot() const {
  return root;
}

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
  vector<Node> result;

  for (char c; input >> c && c != ']'; ) {
    if (c != ',') {
      input.putback(c);
    }
    result.push_back(LoadNode(input));
  }

  return Node(move(result));
}

Node LoadDouble(istream& input, int part) {
  input.get();
  string str = std::to_string(part) + ".";
  while (isdigit(input.peek())) {
    char c; input >> c;
    str += c;
  }
  return Node(stod(str));
}

Node LoadInt(istream& input) {
  int result = 0;
  bool negative = false;
  if (input.peek() == '-') {
    input.get();
    negative = true;
  }
  while (isdigit(input.peek())) {
    result *= 10;
    result += input.get() - '0';
  }
  if (negative)
    result *= -1;
  if (input.peek() == '.')
    return LoadDouble(input, result);
  return Node(result);
}

Node LoadBool(istream& input) {
  char c;
  input >> c;
  if (c == 't') {
    input >> c >> c >> c;
    return Node(true);
  } else {
    input >> c >> c >> c >> c;
    return Node(false);
  }
}

Node LoadString(istream& input) {
  string line;
  getline(input, line, '"');
  return Node(move(line));
}

Node LoadDict(istream& input) {
  map<string, Node> result;

  for (char c; input >> c && c != '}'; ) {
    if (c == ',') {
      input >> c;
    }

    string key = LoadString(input).AsString();
    input >> c;
    result.emplace(move(key), LoadNode(input));
  }

  return Node(move(result));
}

Node LoadNode(istream& input) {
  char c;
  input >> c;

  if (c == '[') {
    return LoadArray(input);
  } else if (c == '{') {
    return LoadDict(input);
  } else if (c == '"') {
    return LoadString(input);
  } else if (c == 't' || c == 'f') {
    input.putback(c);
    return LoadBool(input);
  } else {
    input.putback(c);
    return LoadInt(input);
  }
}

Document Load(istream& input) {
  return Document{LoadNode(input)};
}

string Node::ToString() const {
  if (holds_alternative<int>(*this))
    return to_string(AsInt());
  if (holds_alternative<double>(*this)) {
    stringstream ss;
    ss << setprecision(6) << AsDouble();
    return ss.str();
  }
  if (holds_alternative<string>(*this)) {
    return "\"" + AsString() + "\"";
  }
  if (holds_alternative<bool>(*this)) {
    return AsBool() ? "true" : "false";
  }
  if (holds_alternative<vector<Node>>(*this)) {
    string result = "[";
    bool first = true;
    for (const auto& element : AsArray()) {
      if (!first)
        result += ", ";
      first = false;
      result += element.ToString();
    }
    return result += "]";
  }
  if (holds_alternative<map<string, Node>>(*this)) {
    string result = "{";
    bool first = true;
    for (auto[key, value] : AsMap()) {
      if (!first)
        result += ", ";
      first = false;
      result += Node(key).ToString() + ": " + value.ToString();
    }
    return result += "}";
  }
}

}