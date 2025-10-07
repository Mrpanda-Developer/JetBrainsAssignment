#include "SortedByteTree.hpp"
#include <iostream>

int main() {
    //small test :) 
    SortedByteTree tree;
    tree.put({1, 2, 3}, {10});
    tree.put({1, 2, 4}, {20});
    auto val = tree.get({1, 2, 3});
    std::cout << "Value length: " << val.size() << std::endl;
}
