#ifndef __BM_LIBRARIES_MATH_VECTOR2_HPP__
#define __BM_LIBRARIES_MATH_VECTOR2_HPP__

#include <libraries.lib\math.lib\constant.hpp>
#include <stdexcept>

namespace BM {
namespace math {

template <typename T>
class Vector2 {
    public:
    T x, y;

    inline Vector2() : x(0), y(0) {}
    inline Vector2(T x, T y) : x(x), y(y) {}

    template <typename U> inline explicit Vector2(const Vector2<U>& other) : x(other.x), y(other.y) {}
    template <typename U> inline explicit Vector2(U x, U y) : x(x), y(y) {}

    #ifdef SFML_VECTOR2_HPP
        inline Vector2(const sf::Vector2<T>& vector) : x(vector.x), y(vector.y) {}
        inline Vector2(sf::Vector2<T>* vector) : x(vector->x), y(vector->y) {}
        template <typename U> inline explicit Vector2(const sf::Vector2<U>& vector) : x(vector.x), y(vector.y) {}
        template <typename U> inline explicit Vector2(sf::Vector2<U>* vector) : x(vector->x), y(vector->y) {}

        template <typename U = T> inline sf::Vector2<U> sf() { return sf::Vector2<U>(x, y); }
    #endif

    inline Vector2<T>& operator=(const Vector2<T>& other) = default;
    inline Vector2<T>& operator=(T scalar) { x = scalar; y = scalar; return *this; }

    inline Vector2<T>& operator+=(const Vector2<T>& other) { x += other.x; y += other.y; return *this; }
    inline Vector2<T>& operator-=(const Vector2<T>& other) { x -= other.x; y -= other.y; return *this; }
    inline Vector2<T>& operator*=(const Vector2<T>& other) { x *= other.x; y *= other.y; return *this; }
    inline Vector2<T>& operator/=(const Vector2<T>& other) {
        if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero in vector component");
        x /= other.x; y /= other.y;
        return *this;
    }
    inline Vector2<T>& operator%=(const Vector2<T>& other) {
        if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero in vector component");
        x %= other.x; y %= other.y; 
        return *this; 
    }

    inline Vector2<T>& operator+=(T scalar) { x += scalar; y += scalar; return *this; }
    inline Vector2<T>& operator-=(T scalar) { x -= scalar; y -= scalar; return *this; }
    inline Vector2<T>& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
    inline Vector2<T>& operator/=(T scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        x /= scalar; y /= scalar; 
        return *this; 
    }
    inline Vector2<T>& operator%=(T scalar) {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        x %= scalar; y %= scalar; 
        return *this; 
    }

    inline Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
    inline Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
    inline Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(x * other.x, y * other.y); }
    inline Vector2<T>& operator/(const Vector2<T>& other) { 
        if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero in vector component");
        return Vector2<T>(x / other.x, y / other.y); 
    }
    inline Vector2<T>& operator%(const Vector2<T>& other) { 
        if (other.x == 0 || other.y == 0) throw std::runtime_error("Division by zero in vector component");
        return Vector2<T>(x % other.x, y % other.y); 
    }

    inline Vector2<T> operator+(T scalar) const { return Vector2<T>(x + scalar, y + scalar); }
    inline Vector2<T> operator-(T scalar) const { return Vector2<T>(x - scalar, y - scalar); }
    inline Vector2<T> operator*(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }
    inline Vector2<T> operator/(T scalar) const { 
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Vector2<T>(x / scalar, y / scalar); 
    }
    inline Vector2<T> operator%(T scalar) const { 
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Vector2<T>(x % scalar, y % scalar); 
    }

    inline Vector2<T> operator++() const { return Vector2<T>(x + 1, y + 1); }
    inline Vector2<T> operator--() const { return Vector2<T>(x - 1, y - 1); }

    inline T operator<<(const Vector2<T>& other) const { return x * other.x + y * other.y; }
    inline T operator>>(const Vector2<T>& other) const { return x * other.y - y * other.x; }
    inline T dot(const Vector2<T>& other) const { return x << other; }
    inline T cross(const Vector2<T>& other) const { return x >> other; }

    inline T operator^(const Vector2<T>& other) const { return std::atan2(other.y - y, other.x - x); }
    inline Vector2<T> operator<<(T scaler) const {
        T length = std::hypot(x, y);
        T angle = std::atan2(y, x) + scaler;
        x = length * std::cos(angle);
        y = length * std::sin(angle);
        return *this;
    }
    inline Vector2<T> operator>>(T angle) const {
        T length = std::hypot(x, y);
        x = length * std::cos(angle);
        y = length * std::sin(angle);
        return *this;
    }
    inline Vector2<T> rotate(T angle, bool only_append = false) const {
        return only_append ? *this << angle : *this >> angle;
    }

    inline Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
    inline Vector2<T> operator~() const { return Vector2<T>(-y, x); }
    inline Vector2<T> operator!() const {
        T len = length();
        if (len == 0) throw std::runtime_error("Cannot normalize a zero-length vector");
        return Vector2<T>(x / len, y / len);
    }
    inline Vector2<T> getInverted() const { return -*this; }
    inline Vector2<T> getPerpendicular() const { return ~*this; }
    inline Vector2<T> getNormalized() const { return !*this; }

    inline bool operator==(const Vector2<T>& other) const { return x == other.x && y == other.y; }
    inline bool operator!=(const Vector2<T>& other) const { return !(x == other.x || y == other.y); }
    inline bool operator>(const  Vector2<T>& other) const { return x > other.x && y > other.y; }
    inline bool operator<(const  Vector2<T>& other) const { return x < other.x && y < other.y; }
    inline bool operator>=(const Vector2<T>& other) const { return x >= other.x && y >= other.y; }
    inline bool operator<=(const Vector2<T>& other) const { return x <= other.x && y <= other.y; }

    inline T length() const { return std::hypot(x, y); }
    inline T length_squared() const { return x * x + y * y; }
    inline T distance(const Vector2<T>& other) const { return std::hypot(other.x - x, other.y - y); }
    inline T distance_squared(const Vector2<T>& other) const { return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y); }

    inline T angle() const { return std::atan2(y, x); }
};

using Vector2s =                    Vector2<short>;
using Vector2p =                    Vector2<unsigned short>;
using Vector2i =                    Vector2<int>;
using Vector2u =                    Vector2<unsigned int>;
using Vector2f =                    Vector2<float>;
using Vector2d =                    Vector2<double>;

#define VECTOR2_UP                  Vector2(0, 1)
#define VECTOR2_DOWN                Vector2(0, -1)
#define VECTOR2_LEFT                Vector2(-1, 0)
#define VECTOR2_RIGHT               Vector2(1, 0)
#define VECTOR2_UP_LEFT             Vector2(-1, 1)
#define VECTOR2_UP_RIGHT            Vector2(1, 1)
#define VECTOR2_DOWN_LEFT           Vector2(-1, -1)
#define VECTOR2_DOWN_RIGHT          Vector2(1, -1)
#define VECTOR2_UP_LEFT_SQRT        Vector2(-M_1_SQRT2, M_1_SQRT2)
#define VECTOR2_UP_RIGHT_SQRT       Vector2(M_1_SQRT2, M_1_SQRT2)
#define VECTOR2_DOWN_LEFT_SQRT      Vector2(-M_1_SQRT2, -M_1_SQRT2)
#define VECTOR2_DOWN_RIGHT_SQRT     Vector2(M_1_SQRT2, -M_1_SQRT2)

}
}

#endif
