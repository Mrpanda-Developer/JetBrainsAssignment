#pragma once
#include "Node.hpp"
#include "ByteComparator.hpp"
#include "AVLUtils.hpp"
#include "Locking.hpp"
#include <shared_mutex>

//this is a thread-safe sorted byte tree (AVL tree) implementation that allows insertion, updating, and retrieval of key-value pairs where both keys and values are byte arrays (std::vector<std::uint8_t>).
// The tree maintains its balance using AVL tree rotations to ensure efficient operations.
// It uses a shared mutex to allow concurrent read access while ensuring exclusive access for write operations.
class SortedByteTree {
public:
    using Bytes = std::vector<std::uint8_t>;

    SortedByteTree();
    ~SortedByteTree();

    Bytes put(const Bytes& key, const Bytes& value);
    Bytes get(const Bytes& key) const;

private:
    std::unique_ptr<Node> root_;
    mutable std::shared_mutex rwlock_;

    static Bytes insertOrUpdate(std::unique_ptr<Node>& root,
                                const Bytes& key,
                                const Bytes& value);

    static const Bytes* findNoLock(const std::unique_ptr<Node>& root,
                                   const Bytes& key);
};
