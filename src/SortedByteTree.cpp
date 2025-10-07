#include "SortedByteTree.hpp"

SortedByteTree::SortedByteTree() : root_(nullptr) {}
SortedByteTree::~SortedByteTree() = default;

SortedByteTree::Bytes SortedByteTree::put(const Bytes& key, const Bytes& value) {
    WriteLock lock(rwlock_);
    return insertOrUpdate(root_, key, value);
}

SortedByteTree::Bytes SortedByteTree::get(const Bytes& key) const {
    ReadLock lock(rwlock_);
    const Bytes* found = findNoLock(root_, key);
    return found ? *found : Bytes{};
}

SortedByteTree::Bytes
SortedByteTree::insertOrUpdate(std::unique_ptr<Node>& root,
                               const Bytes& key,
                               const Bytes& value) {
    if (!root) {
        root = std::make_unique<Node>(key, value);
        return {};
    }

    int cmp = ByteComparator::compareUnsigned(key, root->key);
    if (cmp == 0) {
        Bytes prev = root->value;
        root->value = value;
        return prev;
    } else if (cmp < 0) {
        Bytes ret = insertOrUpdate(root->left, key, value);
        AVLUtils::rebalance(root);
        return ret;
    } else {
        Bytes ret = insertOrUpdate(root->right, key, value);
        AVLUtils::rebalance(root);
        return ret;
    }
}

const SortedByteTree::Bytes*
SortedByteTree::findNoLock(const std::unique_ptr<Node>& root,
                           const Bytes& key) {
    const Node* cur = root.get();
    while (cur) {
        int cmp = ByteComparator::compareUnsigned(key, cur->key);
        if (cmp == 0)
            return &cur->value;
        cur = (cmp < 0) ? cur->left.get() : cur->right.get();
    }
    return nullptr;
}
