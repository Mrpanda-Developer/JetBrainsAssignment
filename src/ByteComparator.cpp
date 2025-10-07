#include "ByteComparator.hpp"
// ---------- Comparator (unsigned lexicographic) ----------
int ByteComparator::compareUnsigned(const Bytes& a, const Bytes& b) {
    std::size_t n = std::min(a.size(), b.size());
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
