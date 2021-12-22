//
// Created by JerryGuo on 2021/12/22.
//

#ifndef PARSER_CPP__ULL_NEW_HPP_
#define PARSER_CPP__ULL_NEW_HPP_

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include "Char.hpp"
#include "MemoryRiver.hpp"

#define BLOCK_SIZE 318
#define BLOCK_SPLIT_LIMIT 314
#define BLOCK_SPLIT_LEFT 157
#define BLOCK_MERGE_LIMIT 157
#define KEY_SIZE 65
#define BLOCK_NUM_LIMIT 317

struct Node {
  int value = 0;
  Char<KEY_SIZE> key;

  Node() = default;

  Node(const std::string &, const int &);

  bool operator<(const Node &) const;

  bool operator==(const Node &) const;

  bool operator!=(const Node &) const;

};

template<class NodeType>
class Block {
 private:
  int num = 0;
  NodeType array[BLOCK_SIZE];
 public:
  Block() = default;

  Block(const int &arr_num_, const NodeType *array_);

  const int &Size() const;

  const NodeType &Begin() const;

  const NodeType &End() const;

  bool Add(const NodeType &);

  bool Del(const NodeType &);

  void Split(Block &);

  void Erase(const int &);

  void Merge(Block &);

  void Find(const std::string &, std::vector<int> &) const;
};

template<class NodeType>
struct BlockInfo {
  int position = 0;//存的是在info_length后多少
  NodeType tail;

  BlockInfo() = default;

  BlockInfo(const Block<NodeType> &, int position_ = 0);

  BlockInfo(const NodeType &, int position_ = 0);

  bool operator<(const BlockInfo &) const;
};

template <class NodeType>
struct BlockGallery {
 public:
  int block_num = 1;
  BlockInfo<NodeType> arr[BLOCK_NUM_LIMIT];

  void Add(const BlockInfo<NodeType> &);

  //void Del(const BlockInfo &);

  void Del(const int &);

  void Update(const BlockInfo<NodeType> &, const int &);

  int FindPosition(const NodeType &) const;
};


template<class NodeType>
class UnrolledLinkedList {
 private:
  MemoryRiver<Block<NodeType>, BlockGallery<NodeType>, sizeof(BlockGallery<NodeType>)> block_list;
  BlockGallery<NodeType> block_info;

  void DeleteBlock(const int &);

 public:
  UnrolledLinkedList();

  explicit UnrolledLinkedList(const std::string &);

  void GetInfo();

  void PutInfo();

  bool Add(const NodeType &);

  bool Del(const NodeType &);

  bool Query(const std::string &, std::vector<int> &);
};

#endif //PARSER_CPP__ULL_NEW_HPP_
