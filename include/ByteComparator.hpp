#pragma once
#include <vector>
#include <cstdint>



class ByteComparator {
public:
    using Bytes = std::vector<std::uint8_t>;
    static int compareUnsigned(const Bytes& a, const Bytes& b);
};

// this is just a simple comparator class for comparing byte arrays (std::vector<std::uint8_t>) in an unsigned lexicographic manner.
// It provides a static method compareUnsigned that takes two byte arrays and compares them byte by byte