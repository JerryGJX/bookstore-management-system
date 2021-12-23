//
// Created by JerryGuo on 2021/12/22.
//

#include "Node.hpp"
//...........class Node...........

Node::Node(const std::string &key_, const int &value_) : value(value_) { key = key_; }

Node::Node(const Node &x) {
  key = x.key;
  value = x.value;
}

bool Node::operator<(const Node &x) const {
  if (key != x.key)return key < x.key;
  else return value < x.value;
}

bool Node::operator==(const Node &x) const { return (key == x.key && value == x.value); }

bool Node::operator!=(const Node &x) const { return (key != x.key || value != x.value); }

