#ifndef __BM_LIBRARIES_MATH_VECTOR_HPP__
#define __BM_LIBRARIES_MATH_VECTOR_HPP__

// Cung cấp Vector 2d, 3d, 4d và tùy chỉnh

#include <libraries.lib\math.lib\vector\vector2.hpp>
#include <libraries.lib\math.lib\vector\vector3.hpp>
#include <libraries.lib\math.lib\vector\vector4.hpp>

namespace BM {
namespace math {
    float distance_squared(float a, float b, float x, float y) { return (a - x) * (a - x) + (b - y) * (b - y); }
}
}

#endif
