#ifndef __BM_LIBRARIES_MATH_VECTOR3_HPP__
#define __BM_LIBRARIES_MATH_VECTOR3_HPP__

#include <libraries.lib\math.lib\constant.hpp>
#include <stdexcept>

namespace BM {
namespace math {

template <typename T>
class Vector3 {
public:
    T x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    template <typename U> explicit Vector3(const Vector3<U>& other) : x(other.x), y(other.y), z(other.z) {}
    template <typename U> explicit Vector3(U x, U y, U z) : x(x), y(y), z(z) {}

    #ifdef SFML_VECTOR3_HPP
        Vector3(const sf::Vector3<T>& vector) : x(vector.x), y(vector.y), z(vector.z) {}
        Vector3(sf::Vector3<T>* vector) : x(vector->x), y(vector->y), z(vector->z) {}
        template <typename U> explicit Vector3(const sf::Vector3<U>& vector) : x(vector.x), y(vector.y), z(vector.z) {}
        template <typename U> explicit Vector3(sf::Vector3<U>* vector) : x(vector->x), y(vector->y), z(vector->z) {}

        template <typename U = T> sf::Vector3<U> sf() { return sf::Vector3<U>(x, y, z); }
    #endif

    Vector3<T>& operator=(const Vector3<T>& other) = default;
    Vector3<T>& operator=(T scalar) { x = scalar; y = scalar; z = scalar; return *this; }

    Vector3<T>& operator+=(const Vector3<T>& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3<T>& operator-=(const Vector3<T>& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vector3<T>& operator*=(const Vector3<T>& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
    Vector3<T>& operator/=(const Vector3<T>& other) {
        if (other.x == 0 || other.y == 0 || other.z == 0) throw std::runtime_error("Division by zero in vector component");
        x /= other.x; y /= other.y; z /= other.z;
        return *this;
    }
    Vector3<T>& operator%=(const Vector3<T>& other) {
        if (other.x == 0 || other.y == 0 || other.z == 0) throw std::runtime_error("Division by zero in vector component");
        x %= other.x; y %= other.y; z %= other.z;
        return *this;
    }

    Vector3<T>& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; return *this; }
    Vector3<T>& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
    Vector3<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3<T>& operator/=(T scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }
    Vector3<T>& operator%=(T scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        x %= scalar; y %= scalar; z %= scalar;
        return *this;
    }

    Vector3<T> operator+(const Vector3<T>& other) const { return Vector3<T>(x + other.x, y + other.y, z + other.z); }
    Vector3<T> operator-(const Vector3<T>& other) const { return Vector3<T>(x - other.x, y - other.y, z - other.z); }
    Vector3<T> operator*(const Vector3<T>& other) const { return Vector3<T>(x * other.x, y * other.y, z * other.z); }
    Vector3<T> operator/(const Vector3<T>& other) const {
        if (other.x == 0 || other.y == 0 || other.z == 0) throw std::runtime_error("Division by zero in vector component");
        return Vector3<T>(x / other.x, y / other.y, z / other.z);
    }
    Vector3<T> operator%(const Vector3<T>& other) const {
        if (other.x == 0 || other.y == 0 || other.z == 0) throw std::runtime_error("Division by zero in vector component");
        return Vector3<T>(x % other.x, y % other.y, z % other.z);
    }

    Vector3<T> operator+(T scalar) const { return Vector3<T>(x + scalar, y + scalar, z + scalar); }
    Vector3<T> operator-(T scalar) const { return Vector3<T>(x - scalar, y - scalar, z - scalar); }
    Vector3<T> operator*(T scalar) const { return Vector3<T>(x * scalar, y * scalar, z * scalar); }
    Vector3<T> operator/(T scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Vector3<T>(x / scalar, y / scalar, z / scalar);
    }
    Vector3<T> operator%(T scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Vector3<T>(x % scalar, y % scalar, z % scalar);
    }

    Vector3<T> operator++() { return Vector3<T>(x + 1, y + 1, z + 1); }
    Vector3<T> operator--() { return Vector3<T>(x - 1, y - 1, z - 1); }

    Vector3<T> operator<<(const Vector3<T>& other) const { return x * other.x + y * other.y + z * other.z; }
    Vector3<T> operator>>(const Vector3<T>& other) const { return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
    Vector3<T> dot(const Vector3<T>& other) const { return *this << other; }
    Vector3<T> cross(const Vector3<T>& other) const { return *this >> other; }

    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
    Vector3<T> operator~() const { return Vector3<T>(-y, x, z); }
    Vector3<T> operator!() const {
        T len = length();
        if (len == 0) throw std::runtime_error("Cannot normalize a zero-length vector");
        return Vector2<T>(x / len, y / len, z / len);
    }

    bool operator==(const Vector3<T>& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vector3<T>& other) const { return !(x == other.x || y == other.y || z == other.z); }
    bool operator>(const Vector3<T>& other) const { return x > other.x && y > other.y && z > other.z; }
    bool operator<(const Vector3<T>& other) const { return x < other.x && y < other.y && z < other.z; }
    bool operator>=(const Vector3<T>& other) const { return x >= other.x && y >= other.y && z >= other.z; }
    bool operator<=(const Vector3<T>& other) const { return x <= other.x && y <= other.y && z <= other.z; }

    T length() const { return std::hypot(x, y, z); }
    T length_squared() const { return x * x + y * y + z * z; }
    T distance(const Vector3<T>& other) const { return std::hypot(other.x - x, other.y - y, other.z - z); }
    T distance_squared(const Vector3<T>& other) const { return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z); }
};

using Vector3s = Vector3<short>;
using Vector3p = Vector3<unsigned short>;
using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned int>;
using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

#define VECTOR3_UP                  Vector3(0, 1, 0)
#define VECTOR3_DOWN                Vector3(0, -1, 0)
#define VECTOR3_LEFT                Vector3(-1, 0, 0)
#define VECTOR3_RIGHT               Vector3(1, 0, 0)
#define VECTOR3_FRONT               Vector3(0, 0, 1)
#define VECTOR3_BACK                Vector3(0, 0, -1)

#define VECTOR3_UP_LEFT             Vector3(-1, 1, 0)
#define VECTOR3_UP_RIGHT            Vector3(1, 1, 0)
#define VECTOR3_DOWN_LEFT           Vector3(-1, -1, 0)
#define VECTOR3_DOWN_RIGHT          Vector3(1, -1, 0)
#define VECTOR3_UP_FRONT            Vector3(0, 1, 1)
#define VECTOR3_UP_BACK             Vector3(0, 1, -1)
#define VECTOR3_DOWN_FRONT          Vector3(0, -1, 1)
#define VECTOR3_DOWN_BACK           Vector3(0, -1, -1)
#define VECTOR3_LEFT_FRONT          Vector3(-1, 0, 1)
#define VECTOR3_LEFT_BACK           Vector3(-1, 0, -1)
#define VECTOR3_RIGHT_FRONT         Vector3(1, 0, 1)
#define VECTOR3_RIGHT_BACK          Vector3(1, 0, -1)

#define VECTOR3_UP_LEFT_SQRT        Vector3(-M_1_SQRT2, M_1_SQRT2, 0)
#define VECTOR3_UP_RIGHT_SQRT       Vector3(M_1_SQRT2, M_1_SQRT2, 0)
#define VECTOR3_DOWN_LEFT_SQRT      Vector3(-M_1_SQRT2, -M_1_SQRT2, 0)
#define VECTOR3_DOWN_RIGHT_SQRT     Vector3(M_1_SQRT2, -M_1_SQRT2, 0)
#define VECTOR3_UP_FRONT_SQRT       Vector3(0, M_1_SQRT2, M_1_SQRT2)
#define VECTOR3_UP_BACK_SQRT        Vector3(0, M_1_SQRT2, -M_1_SQRT2)
#define VECTOR3_DOWN_FRONT_SQRT     Vector3(0, -M_1_SQRT2, M_1_SQRT2)
#define VECTOR3_DOWN_BACK_SQRT      Vector3(0, -M_1_SQRT2, -M_1_SQRT2)
#define VECTOR3_LEFT_FRONT_SQRT     Vector3(-M_1_SQRT2, 0, M_1_SQRT2)
#define VECTOR3_LEFT_BACK_SQRT      Vector3(-M_1_SQRT2, 0, -M_1_SQRT2)
#define VECTOR3_RIGHT_FRONT_SQRT    Vector3(M_1_SQRT2, 0, M_1_SQRT2)
#define VECTOR3_RIGHT_BACK_SQRT     Vector3(M_1_SQRT2, 0, -M_1_SQRT2)

#define VECTOR3_UP_LEFT_FRONT       Vector3(-1, 1, 1)
#define VECTOR3_UP_LEFT_BACK        Vector3(-1, 1, -1)
#define VECTOR3_UP_RIGHT_FRONT      Vector3(1, 1, 1)
#define VECTOR3_UP_RIGHT_BACK       Vector3(1, 1, -1)
#define VECTOR3_DOWN_LEFT_FRONT     Vector3(-1, -1, 1)
#define VECTOR3_DOWN_LEFT_BACK      Vector3(-1, -1, -1)
#define VECTOR3_DOWN_RIGHT_FRONT    Vector3(1, -1, 1)
#define VECTOR3_DOWN_RIGHT_BACK     Vector3(1, -1, -1)

#define VECTOR3_UP_LEFT_FRONT_SQRT   Vector3(-M_1_SQRT3, M_1_SQRT3, M_1_SQRT3)
#define VECTOR3_UP_LEFT_BACK_SQRT    Vector3(-M_1_SQRT3, M_1_SQRT3, -M_1_SQRT3)
#define VECTOR3_UP_RIGHT_FRONT_SQRT  Vector3(M_1_SQRT3, M_1_SQRT3, M_1_SQRT3)
#define VECTOR3_UP_RIGHT_BACK_SQRT   Vector3(M_1_SQRT3, M_1_SQRT3, -M_1_SQRT3)
#define VECTOR3_DOWN_LEFT_FRONT_SQRT Vector3(-M_1_SQRT3, -M_1_SQRT3, M_1_SQRT3)
#define VECTOR3_DOWN_LEFT_BACK_SQRT  Vector3(-M_1_SQRT3, -M_1_SQRT3, -M_1_SQRT3)
#define VECTOR3_DOWN_RIGHT_FRONT_SQRT Vector3(M_1_SQRT3, -M_1_SQRT3, M_1_SQRT3)
#define VECTOR3_DOWN_RIGHT_BACK_SQRT  Vector3(M_1_SQRT3, -M_1_SQRT3, -M_1_SQRT3)

}
}

#endif
