#include "SortedByteTree.hpp"
#include <iostream>
#include <thread>

static SortedByteTree::Bytes B(std::initializer_list<uint8_t> il) {
    return SortedByteTree::Bytes(il);
}

int main() {
    SortedByteTree tree;
    tree.put(B({1,2,3}), B({10}));
    tree.put(B({1,2,4}), B({20}));
    tree.put(B({1}),     B({30}));

    auto v = tree.get(B({1,2,3}));
    std::cout << "Value: " << (v.empty() ? -1 : (int)v[0]) << "\n";

    // Threaded smoke test
    std::thread writer([&](){
        for (int i = 0; i < 500; ++i)
            tree.put({(uint8_t)i}, {(uint8_t)(i*2)});
    });
    std::thread reader([&](){
        for (int i = 0; i < 500; ++i)
            tree.get({(uint8_t)i});
    });

    writer.join();
    reader.join();
    std::cout << "Finished.\n";
}
