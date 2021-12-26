//
// Created by JerryGuo on 2021/12/22.
//

#ifndef MAIN_CPP__NODE_HPP_
#define MAIN_CPP__NODE_HPP_

#include <ostream>
#include "Char.hpp"
#define KEY_SIZE 65

struct Node {
  int value = 0;
  Char<KEY_SIZE> key;
  Node() = default;
  Node(const std::string &, const int &);
  Node(const Node &);
  bool operator<(const Node &) const;
  bool operator==(const Node &) const;
  bool operator!=(const Node &) const;
};

struct NodeDigit {
  int value = 0;
  Char<KEY_SIZE> key;
  NodeDigit() = default;
  NodeDigit(const std::string &key_, const int &value_);
  NodeDigit(const NodeDigit &);
  bool operator<(const NodeDigit &) const;
  bool operator==(const NodeDigit &) const;
  bool operator!=(const NodeDigit &) const;
};

struct LongLongWrapper {
  long long a = 0;
  LongLongWrapper() = default;
  LongLongWrapper(const long long &);
  LongLongWrapper(LongLongWrapper &x);
  operator long long() const { return a; };
  bool operator==(const LongLongWrapper &rhs) const;
  bool operator!=(const LongLongWrapper &rhs) const;
  LongLongWrapper &operator=(const LongLongWrapper &rhs);
  friend std::ostream &operator<<(std::ostream &os, const LongLongWrapper &wrapper);
  bool operator<(const LongLongWrapper &rhs) const;
  bool operator>(const LongLongWrapper &rhs) const;
  bool operator<=(const LongLongWrapper &rhs) const;
  bool operator>=(const LongLongWrapper &rhs) const;
  const LongLongWrapper operator++(int);
};

#endif //MAIN_CPP__NODE_HPP_
