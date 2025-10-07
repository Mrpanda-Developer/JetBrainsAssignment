# JetBrainsAssignment

This project presents the implementation of a **thread-safe, in-memory, sorted tree** that stores key–value pairs, where both the keys and values are represented as `byte[]` (implemented as `std::vector<uint8_t>` in C++).  
The design adheres strictly to the assignment requirement of supporting only two operations:  
- `put(key, value)` — inserts or updates a key–value pair  
- `get(key)` — retrieves the value associated with a given key  

The structure is entirely self-implemented, without delegating core functionality to existing container classes such as `std::map` or `std::set`. The primary focus of the project is on correctness, deterministic ordering, and safe concurrent access.

---

## 1. Choice of Data Structure: AVL Tree

First, an AVL tree is a **self-balancing binary search tree** that guarantees logarithmic time complexity (`O(log n)`) for both insertion and lookup in the worst case. Unlike an unbalanced binary search tree, which can degenerate into a linear chain, the AVL tree enforces a strict balance criterion: for every node, the height difference between its left and right subtrees is at most one. This invariant ensures consistent performance regardless of input distribution.

From a theoretical standpoint, AVL trees are among the earliest and most rigorously analyzed balanced tree structures. Their well-defined rotation rules make them predictable and easier to formally reason about compared to more relaxed balancing strategies such as red–black trees.  
While red–black trees can offer slightly faster insertions in practice due to fewer rotations, AVL trees provide superior search performance due to tighter balancing—an important aspect for a structure emphasizing sorted retrieval.

The decision to implement an AVL tree rather than, for example, a skip list or B-tree was also influenced by the **scope of the problem**. The assignment specifies an in-memory, single-process context rather than a paged or disk-based one. In this setting, AVL trees provide a natural balance between **simplicity, determinism, and asymptotic optimality**.

---

## 2. Thread Safety Considerations

Thread safety is achieved through the use of a **single `std::shared_mutex`**, providing coarse-grained synchronization.  
- The `get()` method acquires a *shared lock*, allowing multiple readers to access the tree concurrently.  
- The `put()` method acquires an *exclusive lock*, preventing simultaneous writes or write–read conflicts.

This design ensures correctness and linearizability while maintaining reasonable concurrency for read-heavy workloads.  
Although finer-grained locking (for example, per-node synchronization) could improve parallelism, the chosen approach offers conceptual clarity and avoids the complexity of deadlock prevention and lock ordering.

---

## 3. Implementation Overview

Each node of the tree is defined as a self-contained structure holding:
- a key (`std::vector<uint8_t>`),
- a value (`std::vector<uint8_t>`),
- pointers to left and right subtrees (`std::unique_ptr<Node>`),
- and its height (for balance computation).

The use of `std::unique_ptr` ensures deterministic ownership semantics and automatic memory management without manual deallocation.

Keys are ordered lexicographically using a **custom unsigned byte comparator**, which ensures a total and deterministic order across all possible `byte[]` sequences.  
This ordering rule treats bytes as unsigned values (0–255) and compares them sequentially until a difference is found, or shorter arrays are ordered before longer ones when equal prefixes occur.

Balancing is maintained through the standard AVL rotation operations:
- left rotation,
- right rotation,
- left–right rotation, and
- right–left rotation.

These operations are implemented explicitly without reliance on any standard library tree algorithms.

---

## 5. Building and Running

The project uses **CMake** as its build system.

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build .

# On Windows
cd bin/Debug
./main_test.exe

# On Linux or macOS
cd bin
./main_test


