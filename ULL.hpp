//
// Created by JerryGuo on 2021/12/11.
//

#ifndef UNTITLED1_ULL_HPP
#define UNTITLED1_ULL_HPP

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include "Char.hpp"
#include "MemoryRiver.hpp"


#define BLOCK_SIZE 340
#define BLOCK_SPLIT_LIMIT 300
#define BLOCK_SPLIT_LEFT 150
#define BLOCK_MERGE_LIMIT 30
#define KEY_SIZE 65
#define BLOCK_NUM_LIMIT 350

struct Node {
    int value;
    Char<KEY_SIZE> key;

    Node() = default;

    Node(const std::string &, const int &);

    bool operator<(const Node &) const;

    bool operator==(const Node &) const;

    bool operator!=(const Node &) const;

};

class Block {
private:
    int num = 0;
    Node array[BLOCK_SIZE];
public:
    Block()=default;

    Block(const int &arr_num_, const Node *array_);

    const int &Size() const;

    const Node &Begin() const;

    const Node &End() const;

    bool Add(const Node &);

    bool Del(const Node &);

    Block Split();

    void Erase(const int &);

    Block &Merge(Block &);

    void Find(const std::string &, std::vector<int> &) const;
};

struct BlockInfo {
    int position;//存的是在info_length后多少
    Node tail;

    BlockInfo() = default;

    BlockInfo(const Block &, int position_ = 0);

    BlockInfo(const Node &, int position_ = 0);

    bool operator<(const BlockInfo &) const;
};

struct BlockGallery {
public:
    int block_num = 1;
    BlockInfo arr[BLOCK_NUM_LIMIT];

    void Add(const BlockInfo &);

    void Del(const BlockInfo &);

    void Del(const int &);

    void Update(const BlockInfo &, const int &);

    int FindPosition(const Node &) const;
};

class UnrolledLinkedList {
private:
    MemoryRiver<Block, BlockGallery, sizeof(BlockGallery)> block_list;
    BlockGallery block_info;

    void DeleteBlock(const int &);

public:
    UnrolledLinkedList();

    explicit UnrolledLinkedList(const std::string &);

    void GetInfo();

    void PutInfo();

    bool Add(const Node &);

    bool Del(const Node &);

    bool Query(const std::string &, std::vector<int> &);
};


#endif //UNTITLED1_ULL_HPP
