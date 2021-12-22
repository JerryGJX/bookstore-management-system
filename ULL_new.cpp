//
// Created by JerryGuo on 2021/12/22.
//

#include "ULL_new.hpp"

//...........class Node...........

Node::Node(const std::string &key_, const int &value_) : value(value_) { key = key_; }

bool Node::operator<(const Node &x) const {
  if (key != x.key)return key < x.key;
  else return value < x.value;
}

bool Node::operator==(const Node &x) const { return (key == x.key && value == x.value); }

bool Node::operator!=(const Node &x) const { return (key != x.key || value != x.value); }

//............class block..............
template<class NodeType>
Block<NodeType>::Block(const int &arr_num_, const NodeType *array_) : num(arr_num_) {
  memcpy(array, array_, (num) * sizeof(NodeType));
}
template<class NodeType>
const int &Block<NodeType>::Size() const { return num; }
template<class NodeType>
const NodeType &Block<NodeType>::Begin() const { return array[0]; }
template<class NodeType>
const NodeType &Block<NodeType>::End() const { return (num != 0) ? array[num - 1] : array[0]; }
template<class NodeType>
bool Block<NodeType>::Add(const NodeType &x) {
  int pos = static_cast<int>(std::lower_bound(array, array + num, x) - array);
  if (array[pos] == x)return false;//插入失败
  else {
    for (int i = num - 1; i >= pos; --i) array[i + 1] = array[i];
    array[pos] = x;
    ++num;
    return true;
  }
}
template<class NodeType>
bool Block<NodeType>::Del(const NodeType &x) {
  int pos = static_cast<int>(std::lower_bound(array, array + num, x) - array);
  if (array[pos] != x) return false;
  else {
    for (int i = pos + 1; i < num; ++i) {
      array[i - 1] = array[i];
    }
    array[--num] = NodeType();
    return true;
  }
}
template<class NodeType>
void Block<NodeType>::Erase(const int &start_point) {
  //for (int i = start_point; i < num; ++i) { array[i] = Node(); }
}
template<class NodeType>
void Block<NodeType>::Split(Block &receiver) {
  int new_block_num = num - BLOCK_SPLIT_LEFT;
  receiver = Block(new_block_num, array + BLOCK_SPLIT_LEFT);
  Erase(BLOCK_SPLIT_LEFT);
  num = BLOCK_SPLIT_LEFT;
}
template<class NodeType>
void Block<NodeType>::Merge(Block &x) {//默认x在this后
  for (int i = num; i < num + x.num; ++i) { array[i] = x.array[i - num]; }
  x.Erase(0);
  num += x.num;
  x.num = 0;
}
template<class NodeType>
void Block<NodeType>::Find(const std::string &key_, std::vector<int> &find_list) const {
  Char<KEY_SIZE> key_carrier(key_);
  for (int i = 0; i < num; ++i) {
    if (key_carrier == array[i].key) {
      find_list.push_back(array[i].value);
//            int value_look=find_list.back();
//            std::cout<<value_look<<std::endl;
    }
  }
}
template<class NodeType>
BlockInfo<NodeType>::BlockInfo(const Block<NodeType> &x, int position_) {
  tail = x.End();
  position = position_;
}
template<class NodeType>
bool BlockInfo<NodeType>::operator<(const BlockInfo<NodeType> &x) const { return tail < x.tail; }
template<class NodeType>
BlockInfo<NodeType>::BlockInfo(const NodeType &x, int position_) {
  tail = x;
  position = position_;
}
template<class NodeType>
void BlockGallery<NodeType>::Add(const BlockInfo<NodeType> &x) {
  int pos;
  //block_num++;
  pos = static_cast<int>(std::lower_bound(arr, arr + block_num - 1, x) - arr);
  if (x.tail < arr[block_num - 1].tail) { pos = pos; } else { pos++; }
  block_num++;
  for (int i = block_num - 1; i >= pos; --i) { arr[i + 1] = arr[i]; }
  arr[pos] = x;
}

//void BlockGallery::Del(const BlockInfo &x) {
//    int pos;
//    pos = static_cast<int>(std::lower_bound(arr, arr + block_num, x) - arr);
//    for (int i = pos; i < block_num - 1; ++i) { arr[i] = arr[i + 1]; }
//    arr[block_num - 1] = BlockInfo();
//    block_num--;
//}
template<class NodeType>
void BlockGallery<NodeType>::Del(const int &index_) {
  for (int i = index_; i < block_num - 1; ++i) { arr[i] = arr[i + 1]; }
  arr[block_num - 1] = BlockInfo<NodeType>();
  block_num--;
}
template<class NodeType>
void BlockGallery<NodeType>::Update(const BlockInfo<NodeType> &x, const int &pos_) { arr[pos_] = x; }
template<class NodeType>
int BlockGallery<NodeType>::FindPosition(const NodeType &x) const {
  int pos;
  pos = static_cast<int>(std::lower_bound(arr, arr + block_num - 1, x) - arr);
  return pos;
}





//.......ULL...........
template<class NodeType>
void UnrolledLinkedList<NodeType>::DeleteBlock(const int &index_) {
  GetInfo();
  block_list.Delete(block_info.arr[index_].position);
  block_info.Del(index_);
  PutInfo();
}
template<class NodeType>
UnrolledLinkedList<NodeType>::UnrolledLinkedList() = default;
template<class NodeType>
UnrolledLinkedList<NodeType>::UnrolledLinkedList(const string &file_name_) : block_list(file_name_) {}
template<class NodeType>
void UnrolledLinkedList<NodeType>::GetInfo() { block_list.GetInfo(block_info); }
template<class NodeType>
void UnrolledLinkedList<NodeType>::PutInfo() { block_list.WriteInfo(block_info); }
template<class NodeType>
bool UnrolledLinkedList<NodeType>::Add(const NodeType &x) {
  GetInfo();
  int pos = block_info.FindPosition(x);
  Block<NodeType> target_block;
  block_list.Read(target_block, block_info.arr[pos].position);

//    for (int i = 0; i < block_info.block_num; i++) {
//        Block test_block;
//        block_list.Read(test_block, block_info.arr[i].position);
//        std::cerr << "# " << test_block.Begin().key << '\n';
//        std::cerr << "# " << test_block.Begin().value << '\n';
//        std::cerr << "# " << test_block.End().key << '\n';
//        std::cerr << "# " << test_block.End().value << '\n';
//    }


  if (target_block.Add(x)) {
    block_info.arr[pos].tail = target_block.End();
    if (target_block.Size() >= BLOCK_SPLIT_LIMIT) {
      Block<NodeType> new_block;
      target_block.Split(new_block);
      BlockInfo<NodeType> new_block_info;
      new_block_info.tail = new_block.End();
      new_block_info.position = block_list.Write(new_block);
      block_info.arr[pos].tail = target_block.End();
      block_info.Add(new_block_info);
    }
    block_list.Update(target_block, block_info.arr[pos].position);
    PutInfo();
    return true;
  }
  return false;
}
template<class NodeType>
bool UnrolledLinkedList<NodeType>::Del(const NodeType &x) {
  GetInfo();
  int pos = block_info.FindPosition(x);
  Block<NodeType> target_block;
  block_list.Read(target_block, block_info.arr[pos].position);

//    for (int i = 0; i < block_info.block_num; i++) {
//        Block test_block;
//        block_list.Read(test_block, block_info.arr[i].position);
//        std::cerr << "# " << test_block.Begin().key << '\n';
//        std::cerr << "# " << test_block.Begin().value << '\n';
//        std::cerr << "# " << test_block.End().key << '\n';
//        std::cerr << "# " << test_block.End().value << '\n';
//    }

  if (target_block.Del(x)) {
    if (pos != (block_info.block_num - 1)) {
      block_info.arr[pos].tail = target_block.End();
      Block<NodeType> next_block;
      block_list.Read(next_block, block_info.arr[pos + 1].position);
      if (target_block.Size() <= BLOCK_MERGE_LIMIT &&
          target_block.Size() + next_block.Size() <= BLOCK_SPLIT_LIMIT) {
        target_block.Merge(next_block);
        block_list.Delete(block_info.arr[pos + 1].position);
        block_info.Del(pos + 1);
        block_list.Update(target_block, block_info.arr[pos].position);
        block_info.arr[pos].tail = target_block.End();
        PutInfo();
        return true;
      }
    }
    if (target_block.Size() == 0 && block_info.block_num > 1) {
      block_list.Delete(block_info.arr[pos].position);
      block_info.Del(pos);
      PutInfo();
      return true;
    }
    block_list.Update(target_block, block_info.arr[pos].position);
    PutInfo();
    return true;
  }
  return false;
}
template<class NodeType>
bool UnrolledLinkedList<NodeType>::Query(const string &key_, std::vector<int> &find_list_) {
  GetInfo();
  Char<KEY_SIZE> key_carrier(key_);
  int lp, rp;
  rp = block_info.block_num - 1;
  lp = 0;
  Block<NodeType> this_block;
  for (int i = 0; i < block_info.block_num; ++i) {
    if (key_carrier > block_info.arr[i].tail.key) { lp = i; }
    if (key_carrier < block_info.arr[i].tail.key) {
      rp = i;
      break;
    }
  }
  for (int i = lp; i < rp + 1; i++) {
    block_list.Read(this_block, block_info.arr[i].position);
    this_block.Find(key_, find_list_);
  }
  return find_list_.empty();
}




