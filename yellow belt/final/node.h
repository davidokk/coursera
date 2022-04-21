#pragma once
#include "date.h"
#include <memory>

enum class LogicalOperation {
    And, Or
};

enum class Comparison {
    Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual
};

class Node {
public:
    virtual bool Evaluate(const Date& date, const string& event) = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date &date, const string &event) override;
};

class DateComparisonNode : public Node {
private:
    Comparison cmp;
    Date date;
public:
    DateComparisonNode(const Comparison& new_cmp, const Date& new_date);
    bool Evaluate(const Date& cur_date, const string& event) override;
};

class EventComparisonNode : public Node {
private:
    Comparison cmp;
    string event;
public:
    EventComparisonNode(Comparison new_cmp, string new_event);
    bool Evaluate(const Date& event, const string& cur_event) override;
};

class LogicalOperationNode : public Node {
private:
    LogicalOperation logical_operation;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
public:
    LogicalOperationNode(LogicalOperation new_operation, shared_ptr<Node> new_left, shared_ptr<Node> new_right);
    bool Evaluate(const Date& date, const string& event) override;
};