#include "ULL.hpp"

//...........class Node...........

Node::Node(const std::string &key_, const int &value_) : value(value_) { key = key_; }

bool Node::operator<(const Node &x) const {
    if (key != x.key)return key < x.key;
    else return value < x.value;
}

bool Node::operator==(const Node &x) const { return (key == x.key && value == x.value); }

bool Node::operator!=(const Node &x) const { return (key != x.key || value != x.value); }

//............class block..............
Block::Block(const int &arr_num_, const Node *array_) : num(arr_num_) {
    memcpy(array, array_, (num) * sizeof(Node));
}

const int &Block::Size() const { return num; }

const Node &Block::Begin() const { return array[0]; }

const Node &Block::End() const { return (num != 0) ? array[num - 1] : array[0]; }

bool Block::Add(const Node &x) {
    int pos = static_cast<int>(std::lower_bound(array, array + num, x) - array);
    if (array[pos] == x)return false;//插入失败
    else {
        for (int i = num - 1; i >= pos; --i) array[i + 1] = array[i];
        array[pos] = x;
        ++num;
        return true;
    }
}

bool Block::Del(const Node &x) {
    int pos = static_cast<int>(std::lower_bound(array, array + num, x) - array);
    if (array[pos] != x) return false;
    else {
        for (int i = pos+1; i <num; ++i) {
            array[i - 1] = array[i];
        }
        array[num-1] = Node();
        --num;
        return true;
    }
}

void Block::Erase(const int &start_point) {
    for (int i = start_point; i < num; ++i) { array[i] = Node(); }
}

void Block::Split(Block &receiver) {
    int new_block_num = num - BLOCK_SPLIT_LEFT;
    receiver = Block(new_block_num, array+BLOCK_SPLIT_LEFT);
    Erase(BLOCK_SPLIT_LEFT);num = BLOCK_SPLIT_LEFT;
}

Block &Block::Merge(Block &x) {//默认x在this后
    for (int i = num; i < num + x.num; ++i) { array[i] = x.array[i - num]; }
    x.Erase(0);
    num += x.num;
    x.num = 0;
    return *this;
}

void Block::Find(const std::string &key_, std::vector<int> &find_list) const {
    Char<KEY_SIZE> key_carrier(key_);
    for (int i = 0; i < num; ++i) {
        if (key_carrier == array[i].key) {
            find_list.push_back(array[i].value);
//            int value_look=find_list.back();
//            std::cout<<value_look<<std::endl;
        }

    }
}

BlockInfo::BlockInfo(const Block &x, int position_) {
    tail = x.End();
    position = position_;
}

bool BlockInfo::operator<(const BlockInfo &x) const { return tail < x.tail; }

BlockInfo::BlockInfo(const Node &x, int position_) {
    tail = x;
    position = position_;
}

void BlockGallery::Add(const BlockInfo &x) {
    int pos;
    block_num++;
    pos = static_cast<int>(std::lower_bound(arr, arr + block_num, x) - arr);
    for (int i = block_num - 1; i >= pos; --i) { arr[i + 1] = arr[i]; }
    arr[pos] = x;
    block_num++;
}

void BlockGallery::Del(const BlockInfo &x) {
    int pos;
    pos = static_cast<int>(std::lower_bound(arr, arr + block_num, x) - arr);
    for (int i = pos; i < block_num - 1; ++i) { arr[i] = arr[i + 1]; }
    arr[block_num - 1] = BlockInfo();
    block_num--;
}

void BlockGallery::Del(const int &index_) {
    for (int i = index_; i < block_num - 1; ++i) { arr[i] = arr[i + 1]; }
    arr[block_num - 1] = BlockInfo();
    block_num--;
}

void BlockGallery::Update(const BlockInfo &x, const int &pos_) { arr[pos_] = x; }

int BlockGallery::FindPosition(const Node &x) const {
    int pos;
    pos = static_cast<int>(std::lower_bound(arr, arr + block_num, x) - arr);
    pos = (pos == 0) ? pos : pos - 1;
    return pos;
}





//.......ULL...........

void UnrolledLinkedList::DeleteBlock(const int &index_) {
    GetInfo();
    block_list.Delete(block_info.arr[index_].position);
    block_info.Del(index_);
    PutInfo();
}

UnrolledLinkedList::UnrolledLinkedList() = default;

UnrolledLinkedList::UnrolledLinkedList(const string &file_name_) : block_list(file_name_) {}

void UnrolledLinkedList::GetInfo() { block_list.GetInfo(block_info); }

void UnrolledLinkedList::PutInfo() { block_list.WriteInfo(block_info); }

bool UnrolledLinkedList::Add(const Node &x) {
    GetInfo();
    int pos = block_info.FindPosition(x);
    Block target_block;
    block_list.Read(target_block, block_info.arr[pos].position);
//    std::cerr << "# " << target_block.Begin().key << '\n';
//    std::cerr << "# " << target_block.Begin().value << '\n';
//    std::cerr << "# " << target_block.End().key << '\n';
//    std::cerr << "# " << target_block.End().value << '\n';

    if (target_block.Add(x)) {
        block_info.arr[pos].tail = target_block.End();
        if (target_block.Size() >= BLOCK_SPLIT_LIMIT) {
            Block new_block ;
            target_block.Split(new_block);
            BlockInfo new_block_info;
            new_block_info.tail = new_block.End();
            new_block_info.position = block_list.Write(new_block);
            block_info.Add(new_block_info);
        }
        block_list.Update(target_block, block_info.arr[pos].position);
        PutInfo();
        return true;
    }
    return false;
}

bool UnrolledLinkedList::Del(const Node &x) {
    GetInfo();
    int pos = block_info.FindPosition(x);
    Block target_block;
    block_list.Read(target_block, block_info.arr[pos].position);
    if (target_block.Del(x)) {
        block_info.arr[pos].tail = target_block.End();
        if (pos < block_info.block_num - 1) {
            if (target_block.Size() <= BLOCK_MERGE_LIMIT) {
                Block next_block;
                block_list.Read(next_block, block_info.arr[pos + 1].position);
                target_block.Merge(next_block);
                block_list.Delete(block_info.arr[pos + 1].position);
                block_info.Del(pos + 1);
            }
        }
        block_list.Update(target_block, block_info.arr[pos].position);
        PutInfo();
        return true;
    }
    return false;
}

bool UnrolledLinkedList::Query(const string &key_, std::vector<int> &find_list_) {
    GetInfo();
    Char<KEY_SIZE> key_carrier(key_);
    int lp, rp;
    rp = block_info.block_num - 1;
    lp = 0;
    Block this_block;
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




