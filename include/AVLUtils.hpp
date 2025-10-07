#pragma once
#include "Node.hpp"

class AVLUtils {
public:
    // --- AVL helpers ---
    static int heightOf(const std::unique_ptr<Node>& n);
    static void updateHeight(std::unique_ptr<Node>& n);
    static int balanceFactor(const std::unique_ptr<Node>& n);
    static void rotateLeft(std::unique_ptr<Node>& n);
    static void rotateRight(std::unique_ptr<Node>& n);
    static void rebalance(std::unique_ptr<Node>& n);
    //just standard AVL tree Utils 
};
//height of returns the height by passing a unique_ptr to a node 
// I chose a unique_ptr over a raw pointer to avoid null pointer dereference
// and to make ownership clear. 
// If the node is null, it returns 0; otherwise, it returns the node's height.
