#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Man {
 public:
  Man(const string& name, const string& job) : Name(name), Job(job) {}

  virtual void Walk(const string& destination) const {
    cout << Job << ": " << Name << " walks to: " << destination << endl;
  }
  string Log() const { return Job + ": " + Name; }

  const string Name;
  const string Job;
};

class Student : public Man {
 public:
  Student(const string& name, const string& favouriteSong) : Man(name, "Student"),
                                               FavouriteSong(favouriteSong) {}

  void Walk(const string& destination) const override {
    Man::Walk(destination);
    SingSong();
  }

  void Learn() const {
    cout << Log() << " learns" << endl;
  }
  void SingSong() const {
    cout << Log() << " sings a song: " << FavouriteSong << endl;
  }

 private:
  const string FavouriteSong;
};

class Teacher : public Man {
 public:
  Teacher(const string& name, const string& subject) : Man(name, "Teacher"),
                                         Subject(subject) {}

  void Teach() const {
    cout << Log() << " teaches: " << Subject << endl;
  }

 private:
  const string Subject;
};

class Policeman : public Man {
 public:
  explicit Policeman(const string& name) : Man(name, "Policeman") {}

  void Check(const Man& m) const {
    cout << Log() << " checks " << m.Job << ". " << m.Job
         << "'s name is: " << m.Name << endl;
  }
};

void VisitPlaces(const Man& m, const vector<string>& places) {
    for (const auto& place : places)
      m.Walk(place);
}

int main() {
  Teacher t("Jim", "Math");
  Student s("Ann", "We will rock you");
  Policeman p("Bob");

  VisitPlaces(t, {"Moscow", "London"});
  p.Check(s);
  VisitPlaces(s, {"Moscow", "London"});
  return 0;
}