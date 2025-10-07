#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include <shared_mutex>

class SortedByteTree {
public:
    using Bytes = std::vector<std::uint8_t>;

    SortedByteTree();
    ~SortedByteTree();

    // Thread-safe API
    // Returns previous value if key existed; otherwise {}
    Bytes put(const Bytes& key, const Bytes& value);

    // Returns a copy of the value if found; otherwise {}
    Bytes get(const Bytes& key) const;

private:
    struct Node {
        Bytes key;
        Bytes value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        int height;

        Node(const Bytes& k, const Bytes& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    // --- Data ---
    std::unique_ptr<Node> root_;
    mutable std::shared_mutex rwlock_;

    // --- Comparator (unsigned lexicographic) ---
    static int compareUnsigned(const Bytes& a, const Bytes& b);

    // --- AVL helpers ---
    static int heightOf(const std::unique_ptr<Node>& n);
    static void updateHeight(std::unique_ptr<Node>& n);
    static int balanceFactor(const std::unique_ptr<Node>& n);

    static void rotateLeft(std::unique_ptr<Node>& n);
    static void rotateRight(std::unique_ptr<Node>& n);
    static void rebalance(std::unique_ptr<Node>& n);

    // --- Core (no locking) ---
    static SortedByteTree::Bytes insertOrUpdate(std::unique_ptr<Node>& root,
                                                const Bytes& key,
                                                const Bytes& value);
    static const Bytes* findNoLock(const std::unique_ptr<Node>& root,
                                   const Bytes& key);
};
