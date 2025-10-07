#pragma once
#include <vector>
#include <cstdint>
#include <memory>


// Node structure for the AVL tree
// Each node contains a key-value pair, pointers to left and right children, and the height of the node for balancing purposes.

struct Node {
    using Bytes = std::vector<std::uint8_t>;

    Bytes key;
    Bytes value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    int height;

    Node(const Bytes& k, const Bytes& v)
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};
