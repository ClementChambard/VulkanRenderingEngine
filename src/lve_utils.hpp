#ifndef LVEUTILS_INCLUDED_H
#define LVEUTILS_INCLUDED_H

#include <cstddef>
#include <functional>
namespace lve {

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, T const& v, Rest const&... rest) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

}  // namespace lve

#endif  // !LVEUTILS_INCLUDED_H
