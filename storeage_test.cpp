//
// Created by JerryGuo on 2021/12/11.
//
#include <vector>
#include <iostream>
#include <sstream>
#include "ULL.hpp"

using std::string;
using std::cin;

int main() {

    UnrolledLinkedList list("data");
    int n;

    string receiver;
    string op, index;
    int value;
    getline(cin, receiver);
    std::istringstream iss_(receiver);
    iss_ >> n;
    for (int i = 0; i < n; i++) {
        getline(cin, receiver);
        std::istringstream iss(receiver);
        iss >> op;
        if (op == "insert") {
            iss >> index >> value;
            Node carrier(index, value);
            list.Add(carrier);
        } else if (op == "delete") {
            iss >> index >> value;
            Node carrier(index, value);
            list.Del(carrier);
        } else if (op == "find") {
            iss >> index;
            std::vector<int> find_ans;
            list.Query(index, find_ans);
            if (find_ans.empty())std::cout << "null" << std::endl;
            else {
                sort(find_ans.begin(), find_ans.end());
                for (int j = 0; j < find_ans.size(); ++j) {
                    std::cout << find_ans[j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    return 0;
}