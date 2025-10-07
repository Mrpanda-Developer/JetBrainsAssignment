#include "AVLUtils.hpp"

int AVLUtils::heightOf(const std::unique_ptr<Node>& n) {
    return n ? n->height : 0;
}

void AVLUtils::updateHeight(std::unique_ptr<Node>& n) {
    if (!n) return;
    int hl = heightOf(n->left);
    int hr = heightOf(n->right);
    n->height = ((hl > hr) ? hl : hr) + 1;
}

int AVLUtils::balanceFactor(const std::unique_ptr<Node>& n) {
    return n ? (heightOf(n->left) - heightOf(n->right)) : 0;
}

void AVLUtils::rotateLeft(std::unique_ptr<Node>& n) {
    std::unique_ptr<Node> R = std::move(n->right);
    std::unique_ptr<Node> Rl = std::move(R->left);
    R->left = std::move(n);
    R->left->right = std::move(Rl);
    updateHeight(R->left);
    updateHeight(R);
    n = std::move(R);
}

void AVLUtils::rotateRight(std::unique_ptr<Node>& n) {
    std::unique_ptr<Node> L = std::move(n->left);
    std::unique_ptr<Node> Lr = std::move(L->right);
    L->right = std::move(n);
    L->right->left = std::move(Lr);
    updateHeight(L->right);
    updateHeight(L);
    n = std::move(L);
}

void AVLUtils::rebalance(std::unique_ptr<Node>& n) {
    if (!n) return;
    updateHeight(n);
    int bf = balanceFactor(n);
    if (bf > 1) {
        if (balanceFactor(n->left) < 0)
            rotateLeft(n->left);
        rotateRight(n);
    } else if (bf < -1) {
        if (balanceFactor(n->right) > 0)
            rotateRight(n->right);
        rotateLeft(n);
    }
}
