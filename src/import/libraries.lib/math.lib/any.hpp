#ifndef __BM_LIBRARIES_MATH_ANY_HPP__
#define __BM_LIBRARIES_MATH_ANY_HPP__

namespace BM {
namespace math {
    template <typename T>
    T abs(T x) { return x > 0 ? x : -x; }

    template <typename T>
    T sign(T x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

    template <typename T>
    T max(T x, T y) { return x > y ? x : y; }

    template <typename T>
    T min(T x, T y) { return x < y ? x : y; }
}
}

#endif
