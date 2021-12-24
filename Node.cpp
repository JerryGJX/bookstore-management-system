//
// Created by JerryGuo on 2021/12/22.
//

#include "Node.hpp"
//...........class Node...........

Node::Node(const std::string &key_, const int &value_) {
  //memset(this, 0, sizeof(Node));
  key = key_;
  value = value_;
}

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

//...............NodeDigit...............
NodeDigit::NodeDigit(const std::string &key_, const int &value_) {
  key = key_;
  value = value_;
}
NodeDigit::NodeDigit(const NodeDigit &x) {
  key = x.key;
  value = x.value;
}
bool NodeDigit::operator<(const NodeDigit &x) const {
  if (key != x.key) {
    int key_ = std::stoi(key);
    int key_x = std::stoi(x.key);
    return key_ < key_x;
  } else return value < x.value;
}
bool NodeDigit::operator==(const NodeDigit &x) const { return (key == x.key && value == x.value); }

bool NodeDigit::operator!=(const NodeDigit &x) const { return (key != x.key || value != x.value); }

IntWrapper::IntWrapper(const int &x) {
  a = x;
}
IntWrapper::IntWrapper(IntWrapper &x) {
  a = x.a;
}
bool IntWrapper::operator==(const IntWrapper &rhs) const {
  return a == rhs.a;
}
bool IntWrapper::operator!=(const IntWrapper &rhs) const {
  return !(rhs == *this);
}
IntWrapper &IntWrapper::operator=(const IntWrapper &rhs) = default;
std::ostream &operator<<(std::ostream &os, const IntWrapper &wrapper) {
  os << wrapper.a;
  return os;
}
bool IntWrapper::operator<(const IntWrapper &rhs) const {
  return a < rhs.a;
}
bool IntWrapper::operator>(const IntWrapper &rhs) const {
  return rhs < *this;
}
bool IntWrapper::operator<=(const IntWrapper &rhs) const {
  return !(rhs < *this);
}
bool IntWrapper::operator>=(const IntWrapper &rhs) const {
  return !(*this < rhs);
}
IntWrapper IntWrapper::operator++(int) {
  IntWrapper carrier;
  carrier=*this;
  a++;
  return carrier;
}
