#include "SortedByteTree.hpp"
#include <algorithm>
#include <stdexcept>

// ---------- Public ----------

SortedByteTree::SortedByteTree() : root_(nullptr) {}
SortedByteTree::~SortedByteTree() = default;

SortedByteTree::Bytes SortedByteTree::put(const Bytes& key, const Bytes& value) {
    // Defensive: reject null-equivalent? Here we allow empty arrays.
    std::unique_lock<std::shared_mutex> lock(rwlock_);
    return insertOrUpdate(root_, key, value);
}

SortedByteTree::Bytes SortedByteTree::get(const Bytes& key) const {
    std::shared_lock<std::shared_mutex> lock(rwlock_);
    const Bytes* found = findNoLock(root_, key);
    if (!found) return {};
    return *found; // copy out to avoid exposing internal storage
}

// ---------- Comparator (unsigned lexicographic) ----------

int SortedByteTree::compareUnsigned(const Bytes& a, const Bytes& b) {
    const std::size_t n = (a.size() < b.size()) ? a.size() : b.size();
    for (std::size_t i = 0; i < n; ++i) {
        std::uint8_t ai = a[i];
        std::uint8_t bi = b[i];
        if (ai < bi) return -1;
        if (ai > bi) return +1;
    }
    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return +1;
    return 0;
}

// ---------- AVL helpers ----------

int SortedByteTree::heightOf(const std::unique_ptr<Node>& n) {
    return n ? n->height : 0;
}

void SortedByteTree::updateHeight(std::unique_ptr<Node>& n) {
    if (!n) return;
    int hl = heightOf(n->left);
    int hr = heightOf(n->right);
    n->height = ((hl > hr) ? hl : hr) + 1;
}

int SortedByteTree::balanceFactor(const std::unique_ptr<Node>& n) {
    return n ? (heightOf(n->left) - heightOf(n->right)) : 0;
}

// n points to subtree root (unique_ptr reference)
// Perform: n = rotateLeft(n)
void SortedByteTree::rotateLeft(std::unique_ptr<Node>& n) {
    //    n                   R
    //     \                 / \
    //      R      =>       n  Rr
    //     / \               \
    //   Rl  Rr              Rl
    std::unique_ptr<Node> R = std::move(n->right);
    std::unique_ptr<Node> Rl = std::move(R->left);

    R->left = std::move(n);
    R->left->right = std::move(Rl);

    updateHeight(R->left);
    updateHeight(R);
    n = std::move(R);
}

void SortedByteTree::rotateRight(std::unique_ptr<Node>& n) {
    //        n              L
    //       /              / \
    //      L      =>      Ll  n
    //     / \                 /
    //   Ll  Lr              Lr
    std::unique_ptr<Node> L = std::move(n->left);
    std::unique_ptr<Node> Lr = std::move(L->right);

    L->right = std::move(n);
    L->right->left = std::move(Lr);

    updateHeight(L->right);
    updateHeight(L);
    n = std::move(L);
}

void SortedByteTree::rebalance(std::unique_ptr<Node>& n) {
    if (!n) return;
    updateHeight(n);
    int bf = balanceFactor(n);

    if (bf > 1) {
        // Left heavy
        if (balanceFactor(n->left) < 0) {
            // Left-Right
            rotateLeft(n->left);
        }
        rotateRight(n);
    } else if (bf < -1) {
        // Right heavy
        if (balanceFactor(n->right) > 0) {
            // Right-Left
            rotateRight(n->right);
        }
        rotateLeft(n);
    }
}

// ---------- Core (no locking) ----------

SortedByteTree::Bytes
SortedByteTree::insertOrUpdate(std::unique_ptr<Node>& root,
                               const Bytes& key,
                               const Bytes& value) {
    if (!root) {
        root = std::make_unique<Node>(key, value); // copy in
        return {}; // no previous value
    }

    int cmp = compareUnsigned(key, root->key);
    if (cmp == 0) {
        // Replace value, return previous
        Bytes prev = root->value;
        root->value = value; // copy assignment
        return prev;
    } else if (cmp < 0) {
        Bytes ret = insertOrUpdate(root->left, key, value);
        rebalance(root);
        return ret;
    } else {
        Bytes ret = insertOrUpdate(root->right, key, value);
        rebalance(root);
        return ret;
    }
}

const SortedByteTree::Bytes*
SortedByteTree::findNoLock(const std::unique_ptr<Node>& root,
                           const Bytes& key) {
    const Node* cur = root.get();
    while (cur) {
        int cmp = compareUnsigned(key, cur->key);
        if (cmp == 0) {
            return &cur->value;
        } else if (cmp < 0) {
            cur = cur->left.get();
        } else {
            cur = cur->right.get();
        }
    }
    return nullptr;
}
