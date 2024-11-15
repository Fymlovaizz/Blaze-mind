#ifndef __BM_LIBRARIES_MATH_CONSTANT_HPP__
#define __BM_LIBRARIES_MATH_CONSTANT_HPP__

#include <cmath>
#include <limits>

#undef CHAR_MIN
#undef CHAR_MAX
#undef UCHAR_MAX
#undef SHORT_MIN
#undef SHORT_MAX
#undef USHORT_MAX
#undef INT_MIN
#undef INT_MAX
#undef UINT_MAX
#undef LONG_MIN
#undef LONG_MAX
#undef ULONG_MAX
#undef LONG_LONG_MIN
#undef LONG_LONG_MAX
#undef ULONG_LONG_MAX
#undef DOUBLE_MIN
#undef DOUBLE_MAX
#undef LONG_DOUBLE_MIN
#undef LONG_DOUBLE_MAX
#undef FLOAT_MIN
#undef FLOAT_MAX

#define CHAR_MIN                (std::numeric_limits<char>::min())
#define CHAR_MAX                (std::numeric_limits<char>::max())
#define UCHAR_MAX               (std::numeric_limits<unsigned char>::max())
#define SHORT_MIN               (std::numeric_limits<short>::min())
#define SHORT_MAX               (std::numeric_limits<short>::max())
#define USHORT_MAX              (std::numeric_limits<unsigned short>::max())
#define INT_MIN                 (std::numeric_limits<int>::min())
#define INT_MAX                 (std::numeric_limits<int>::max())
#define UINT_MAX                (std::numeric_limits<unsigned int>::max())
#define FLOAT_MIN               (std::numeric_limits<float>::min())
#define FLOAT_MAX               (std::numeric_limits<float>::max())
#define LONG_MIN                (std::numeric_limits<long>::min())
#define LONG_MAX                (std::numeric_limits<long>::max())
#define ULONG_MAX               (std::numeric_limits<unsigned long>::max())
#define LONG_LONG_MIN           (std::numeric_limits<long long>::min())
#define LONG_LONG_MAX           (std::numeric_limits<long long>::max())
#define ULONG_LONG_MAX          (std::numeric_limits<unsigned long long>::max())
#define DOUBLE_MIN              (std::numeric_limits<double>::min())
#define DOUBLE_MAX              (std::numeric_limits<double>::max())
#define LONG_DOUBLE_MIN         (std::numeric_limits<long double>::min())
#define LONG_DOUBLE_MAX         (std::numeric_limits<long double>::max())

#undef M_E
#undef M_LOG2E
#undef M_LOG10E
#undef M_LN2
#undef M_LN10
#undef M_PI
#undef M_PI_2
#undef M_PI_4
#undef M_1_PI
#undef M_2_PI
#undef M_2_SQRTPI
#undef M_SQRT2
#undef M_1_SQRT2
#undef M_SQRT3
#undef M_1_SQRT3

#define M_E                     2.7182818284590452354
#define M_LOG2E                 1.4426950408889634074
#define M_LOG10E                0.43429448190325182765
#define M_LN2                   0.69314718055994530942
#define M_LN10                  2.30258509299404568402
#define M_PI                    3.14159265358979323846
#define M_PI_2                  1.57079632679489661923
#define M_PI_4                  0.78539816339744830962
#define M_1_PI                  0.31830988618379067154
#define M_2_PI                  0.63661977236758134308
#define M_2_SQRTPI              1.12837916709551257390
#define M_SQRT2                 1.41421356237309504880
#define M_1_SQRT2               0.70710678118654752440
#define M_SQRT3                 1.73205080756887729352
#define M_1_SQRT3               0.57735026918962576451

#endif
