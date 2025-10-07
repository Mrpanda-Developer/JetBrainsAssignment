#pragma once
#include <vector>
#include <cstdint>
#include <memory>

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
