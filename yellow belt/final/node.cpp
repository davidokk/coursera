#include "node.h"

DateComparisonNode::DateComparisonNode(const Comparison &new_cmp, const Date& new_date) : date(new_date) {
    cmp = new_cmp;
}

EventComparisonNode::EventComparisonNode(Comparison new_cmp, string new_event) {
    cmp = new_cmp;
    event = std::move(new_event);
}

LogicalOperationNode::LogicalOperationNode(LogicalOperation new_operation, shared_ptr<Node> new_left, shared_ptr<Node> new_right) {
    logical_operation = new_operation;
    left = std::move(new_left);
    right = std::move(new_right);
}

bool EmptyNode::Evaluate(const Date &date, const string &event) {
    return true;
}

bool DateComparisonNode::Evaluate(const Date &cur_date, const string &event) {
    switch (cmp) {
        case Comparison::Less:
            return cur_date < date;
        case Comparison::LessOrEqual:
            return cur_date <= date;
        case Comparison::Greater:
            return cur_date > date;
        case Comparison::GreaterOrEqual:
            return cur_date >= date;
        case Comparison::Equal:
            return cur_date == date;
        case Comparison::NotEqual:
            return cur_date != date;
    }
}

bool EventComparisonNode::Evaluate(const Date &date, const string& cur_event) {
    switch (cmp) {
        case Comparison::Less:
            return cur_event < event;
        case Comparison::LessOrEqual:
            return cur_event <= event;
        case Comparison::Greater:
            return cur_event > event;
        case Comparison::GreaterOrEqual:
            return cur_event >= event;
        case Comparison::Equal:
            return cur_event == event;
        case Comparison::NotEqual:
            return cur_event != event;
    }
}

bool LogicalOperationNode::Evaluate(const Date &date, const string &event) {
    auto l = left->Evaluate(date, event);
    auto r = right->Evaluate(date, event);
    if (logical_operation == LogicalOperation::And)
        return l && r;
    return l || r;
}

