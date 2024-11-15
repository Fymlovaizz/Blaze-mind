#ifndef __BM_LIBRARIES_MATH_VECTOR4_HPP__
#define __BM_LIBRARIES_MATH_VECTOR4_HPP__

#include <stdexcept>
#include <type_traits>

namespace BM {
namespace math {

template<typename T>
class Vector4 {
public:
    T r, g, b, a;

    Vector4() : r(0), g(0), b(0), a(0) {}
    Vector4(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}

    template <typename U> explicit Vector4(const Vector4<U>& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}
    template <typename U> explicit Vector4(U r, U g, U b, U a) : r(r), g(g), b(b), a(a) {}
    
    Vector4<T>& operator=(const Vector4<T>& other) = default;
    Vector4<T>& operator=(T scalar) { r = scalar; g = scalar; b = scalar; a = scalar; return *this; }

    // Operator overloads for +=, -=, *=, /=
    Vector4<T>& operator+=(const Vector4<T>& other) {
        r += other.r; g += other.g; b += other.b; a += other.a;
        return *this;
    }

    Vector4<T>& operator-=(const Vector4<T>& other) {
        r -= other.r; g -= other.g; b -= other.b; a -= other.a;
        return *this;
    }

    Vector4<T>& operator*=(const Vector4<T>& other) {
        r *= other.r; g *= other.g; b *= other.b; a *= other.a;
        return *this;
    }

    Vector4<T>& operator/=(const Vector4<T>& other) {
        if (other.r == 0 || other.g == 0 || other.b == 0 || other.a == 0)
            throw std::runtime_error("Division by zero in vector component");
        r /= other.r; g /= other.g; b /= other.b; a /= other.a;
        return *this;
    }

    // Scalar operations
    Vector4<T>& operator+=(T scalar) {
        r += scalar; g += scalar; b += scalar; a += scalar;
        return *this;
    }

    Vector4<T>& operator-=(T scalar) {
        r -= scalar; g -= scalar; b -= scalar; a -= scalar;
        return *this;
    }

    Vector4<T>& operator*=(T scalar) {
        r *= scalar; g *= scalar; b *= scalar; a *= scalar;
        return *this;
    }

    Vector4<T>& operator/=(T scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        r /= scalar; g /= scalar; b /= scalar; a /= scalar;
        return *this;
    }

    // Vector addition, subtraction, multiplication, division
    Vector4<T> operator+(const Vector4<T>& other) const {
        return Vector4<T>(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    Vector4<T> operator-(const Vector4<T>& other) const {
        return Vector4<T>(r - other.r, g - other.g, b - other.b, a - other.a);
    }

    Vector4<T> operator*(const Vector4<T>& other) const {
        return Vector4<T>(r * other.r, g * other.g, b * other.b, a * other.a);
    }

    Vector4<T> operator/(const Vector4<T>& other) const {
        if (other.r == 0 || other.g == 0 || other.b == 0 || other.a == 0)
            throw std::runtime_error("Division by zero in vector component");
        return Vector4<T>(r / other.r, g / other.g, b / other.b, a / other.a);
    }

    // Scalar operations
    Vector4<T> operator+(T scalar) const {
        return Vector4<T>(r + scalar, g + scalar, b + scalar, a + scalar);
    }

    Vector4<T> operator-(T scalar) const {
        return Vector4<T>(r - scalar, g - scalar, b - scalar, a - scalar);
    }

    Vector4<T> operator*(T scalar) const {
        return Vector4<T>(r * scalar, g * scalar, b * scalar, a * scalar);
    }

    Vector4<T> operator/(T scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Vector4<T>(r / scalar, g / scalar, b / scalar, a / scalar);
    }

    // Increment and Decrement operators
    Vector4<T> operator++() {
        return Vector4<T>(r + 1, g + 1, b + 1, a + 1);
    }

    Vector4<T> operator--() {
        return Vector4<T>(r - 1, g - 1, b - 1, a - 1);
    }

    // Comparison operators
    bool operator==(const Vector4<T>& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const Vector4<T>& other) const {
        return !(r == other.r && g == other.g && b == other.b && a == other.a);
    }

    bool operator>(const Vector4<T>& other) const {
        return r > other.r && g > other.g && b > other.b && a > other.a;
    }

    bool operator<(const Vector4<T>& other) const {
        return r < other.r && g < other.g && b < other.b && a < other.a;
    }

    bool operator>=(const Vector4<T>& other) const {
        return r >= other.r && g >= other.g && b >= other.b && a >= other.a;
    }

    bool operator<=(const Vector4<T>& other) const {
        return r <= other.r && g <= other.g && b <= other.b && a <= other.a;
    }

    // Color
    Vector4<unsigned char> toColor() const { 
        if (std::is_same<T, unsigned char>::value) return *this;
        if (std::is_same<T, float>::value) return Vector4<unsigned char>(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
        else throw std::runtime_error("Invalid color type");
    }
    Vector4<float> toColorF() const {
        if (std::is_same<T, unsigned char>::value) return Vector4<float>(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
        if (std::is_same<T, float>::value) return *this;
        else throw std::runtime_error("Invalid color type");
    }
};

using Color =                       Vector4<unsigned char>;
using ColorF =                      Vector4<float>;

using Vector4s =                    Vector4<short>;
using Vector4p =                    Vector4<unsigned short>;
using Vector4i =                    Vector4<int>;
using Vector4u =                    Vector4<unsigned int>;
using Vector4f =                    Vector4<float>;
using Vector4d =                    Vector4<double>;

}

namespace Color {
    const math::Color Red(255, 0, 0, 255);
    const math::Color Green(0, 255, 0, 255);
    const math::Color Blue(0, 0, 255, 255);
    const math::Color White(255, 255, 255, 255);
    const math::Color Black(0, 0, 0, 255);
    const math::Color Yellow(255, 255, 0, 255);
    const math::Color Magenta(255, 0, 255, 255);
    const math::Color Cyan(0, 255, 255, 255);
    const math::Color Transparent(0, 0, 0, 0);
}

namespace ColorF {
    const math::ColorF Red(1.f, 0.f, 0.f, 1.f);
    const math::ColorF Green(0.f, 1.f, 0.f, 1.f);
    const math::ColorF Blue(0.f, 0.f, 1.f, 1.f);
    const math::ColorF White(1.f, 1.f, 1.f, 1.f);
    const math::ColorF Black(0.f, 0.f, 0.f, 1.f);
    const math::ColorF Yellow(1.f, 1.f, 0.f, 1.f);
    const math::ColorF Magenta(1.f, 0.f, 1.f, 1.f);
    const math::ColorF Cyan(0.f, 1.f, 1.f, 1.f);
    const math::ColorF Transparent(0.f, 0.f, 0.f, 0.f);
}

}

#endif
