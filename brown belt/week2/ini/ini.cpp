#include "ini.h"
using namespace std;

namespace Ini {

Section& Document::AddSection(std::string name) {
  return sections[move(name)];
}

const Section& Document::GetSection(const std::string& name) const {
  return sections.at(name);
}

size_t Document::SectionCount() const {
  return sections.size();
}

Document Load(istream& input) {
  Document document;
  for(std::string line; getline(input, line); ) {
    if (line.empty())
      continue;
    Section& section = document.AddSection(line.substr(1, line.size() - 2));
    while (input.peek() != '[' && getline(input, line)) {
      if (line.empty())
        continue;
      int eq_pos = line.find('=');
      section[line.substr(0, eq_pos)] = line.substr(eq_pos + 1, line.size() - eq_pos);
    }
  }
  return document;
}

}
