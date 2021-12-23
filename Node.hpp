//
// Created by JerryGuo on 2021/12/22.
//

#ifndef MAIN_CPP__NODE_HPP_
#define MAIN_CPP__NODE_HPP_

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
  int key = 0;
  NodeDigit() = default;
  NodeDigit(const int &key_, const int &value_);
  NodeDigit(const NodeDigit &);
  bool operator<(const NodeDigit &) const;
  bool operator==(const NodeDigit &) const;
  bool operator!=(const NodeDigit &) const;
};

#endif //MAIN_CPP__NODE_HPP_
