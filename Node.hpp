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
#endif //MAIN_CPP__NODE_HPP_
