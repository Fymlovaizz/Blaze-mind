#ifndef __BM_LIBRARIES_MATH_RECT_HPP__
#define __BM_LIBRARIES_MATH_RECT_HPP__

namespace BM {
namespace math {

template <typename T>
class Rect {
public:
    T left, right, top, bottom;

    Rect() : 
        left(0), 
        right(0), 
        top(0), 
        bottom(0) 
    {}

    Rect(T left, T right, T top, T bottom) : 
        left(left), 
        right(right), 
        top(top), 
        bottom(bottom) 
    {}

    Rect(const Vector2<T>& position, const Vector2<T>& size) : 
        left(position.x), 
        right(position.x + size.x), 
        top(position.y), 
        bottom(position.y + size.y) 
    {}

    template <typename U>
    explicit Rect(const Rect<U>& rectangle) : 
        left(static_cast<T>(rectangle.left)), 
        right(static_cast<T>(rectangle.right)), 
        top(static_cast<T>(rectangle.top)), 
        bottom(static_cast<T>(rectangle.bottom)) 
    {}

    template <typename U>
    explicit Rect(const U& left, const U& top, const U& right, const U& bottom) : 
        left(static_cast<T>(left)), 
        right(static_cast<T>(right)), 
        top(static_cast<T>(top)), 
        bottom(static_cast<T>(bottom)) 
    {}

    template <typename U>
    explicit Rect(Rect<U>& rectangle) : 
        left(static_cast<T>(rectangle.left)), 
        right(static_cast<T>(rectangle.right)), 
        top(static_cast<T>(rectangle.top)), 
        bottom(static_cast<T>(rectangle.bottom))
    {}

    template <typename U>
    explicit operator Rect<U>() const { 
        return Rect<U>(static_cast<U>(left), static_cast<U>(top), static_cast<U>(right), static_cast<U>(bottom)); 
    }

    bool contains(const Vector2<T>& point) const { 
        return (point.x >= left) && (point.x <= right) && (point.y >= top) && (point.y <= bottom); 
    }

    bool contains(T x, T y) const { 
        return (x >= left) && (x <= right) && (y >= top) && (y <= bottom); 
    }

    Rect intersection(const Rect& rectangle) const { 
        return Rect(
            std::max(left, rectangle.left), 
            std::min(right, rectangle.right), 
            std::max(top, rectangle.top), 
            std::min(bottom, rectangle.bottom)
        ); 
    }

    Rect intersection(const Vector2<T>& position, const Vector2<T>& size) const { 
        return Rect(
            std::max(left, position.x), 
            std::min(right, position.x + size.x), 
            std::max(top, position.y), 
            std::min(bottom, position.y + size.y)
        ); 
    }

    void setPosition(const Vector2<T>& position) { 
        left = position.x; 
        top = position.y; 
    }

    void setSize(const Vector2<T>& size) { 
        right = left + size.x; 
        bottom = top + size.y; 
    }

    Vector2<T> position() const { 
        return Vector2<T>(left, top); 
    }

    Vector2<T> size() const { 
        return Vector2<T>(right - left, bottom - top); 
    }

    T width() const { return right - left; }
    T height() const { return bottom - top; }
};

using IntRect =             Rect<int>;
using UintRect =            Rect<unsigned int>;
using FloatRect =           Rect<float>;
using DoubleRect =          Rect<double>;

}
}

#endif
