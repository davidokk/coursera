#include <iostream>
#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>
using namespace std;

class Figure {
private:
    const string name;
public:
    Figure(const string& new_name) : name(new_name) {}

    string Name() const  {
        return name;
    }
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
private:
    int a, b, c;
public:
    Triangle(int new_a, int new_b, int new_c) : Figure("TRIANGLE") {
        a = new_a;
        b = new_b;
        c = new_c;
    }
    double Perimeter() const override {
        return a + b + c;
    }
    double Area() const override {
        double p = (a + b + c) / double(2);
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }
};

class Rect : public Figure {
private:
    int a, b;
public:
    Rect(int new_a, int new_b) : Figure("RECT") {
        a = new_a;
        b = new_b;
    }
    double Perimeter() const override {
        return 2 * (a + b);
    }
    double Area() const override {
        return a * b;
    }
};

class Circle : public Figure {
private:
    int r;
public:
    Circle(int new_r) : Figure("CIRCLE") {
        r = new_r;
    }
    double Perimeter() const override {
        return 2 * r * 3.14;
    }
    double Area() const override {
        return 3.14 * r * r;
    }
};

shared_ptr<Figure> CreateFigure(istream& is) {
    string type;
    is >> type;
    if (type == "CIRCLE") {
        int r;
        is >> r;
        return make_shared<Circle>(r);
    }
    if (type == "RECT") {
        int a, b;
        is >> a >> b;
        return make_shared<Rect>(a, b);
    }
    if (type == "TRIANGLE") {
        int a, b, c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    }
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            // Пропускаем "лишние" ведущие пробелы.
            // Подробнее об std::ws можно узнать здесь:
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}
