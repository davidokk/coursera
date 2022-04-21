#include "Common.h"

using namespace std;

class Integer : public Expression {
 public:
  explicit Integer(int val) : value(val) {}

  string ToString() const override { return std::to_string(value); }

  int Evaluate() const override { return value; }

 private:
  int value;
};

class Operation : public Expression {
 public:
  Operation(ExpressionPtr l, ExpressionPtr r, bool plus) : left(move(l)), right(move(r)), is_plus(plus) {}

  string ToString() const override {
    return AddBrackets(left->ToString()) + (is_plus? "+" : "*") + AddBrackets(right->ToString());
  }

  int Evaluate() const override {
    return is_plus? left->Evaluate() + right->Evaluate() : left->Evaluate() * right->Evaluate();
  }

 private:
  ExpressionPtr left, right;
  bool is_plus;

  string AddBrackets(string s) const {
    s = "(" + s + ")";
    return move(s);
  }
};

ExpressionPtr Value(int value) { return make_unique<Integer>(value); }
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<Operation>(move(left), move(right), true);
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
  return make_unique<Operation>(move(left), move(right), false);
}