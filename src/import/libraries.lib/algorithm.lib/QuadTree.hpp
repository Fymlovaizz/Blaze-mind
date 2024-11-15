#ifndef __BM_LIBRARIES_ALGORITHM_QUADTREE_HPP__
#define __BM_LIBRARIES_ALGORITHM_QUADTREE_HPP__

#include <libraries.lib/math.lib/vector/vector2.hpp>
#include <libraries.lib/math.lib/rect.hpp>
#include <utility>
#include <vector>

namespace BM {
namespace algorithm {

// Dinh nghia lop QuadTree voi cac tham so kieu T_size va T_value
template <typename T_size, typename T_value>
class QuadTree {
private:
    math::Rect<T_size> bounds; // Gioi han cua cay
    int max_objects; // So luong doi tuong toi da trong moi nut
    int level; // Cap do cua nut trong cay
    QuadTree *nodes[4]; // Mang con tro toi cac nut con
    std::vector<std::pair<math::Vector2<T_size>, T_value*>> objects; // Danh sach cac doi tuong trong nut
    std::vector<math::Rect<T_size>> sizes;
    math::Rect<T_size> biggest;

    sf::RectangleShape line;
public:
    // Constructor - Khoi tao QuadTree
    QuadTree(math::Rect<T_size> bounds, int level = 4, int max_objects = 4)
        : level(level), bounds(bounds), max_objects(max_objects) {
        for (int i = 0; i < 4; ++i) {
            nodes[i] = nullptr; // Khoi tao cac nut con la nullptr
        }

        line.setOutlineThickness(20.f);
        line.setOutlineColor(sf::Color(255, 255, 255, 100));
        line.setPosition(bounds.left, bounds.top);
        line.setFillColor(sf::Color::Transparent);
        line.setSize(sf::Vector2f(bounds.width(), bounds.height()));
    }

    // Destructor - Huy QuadTree
    ~QuadTree() {
        clear(); // Goi ham clear de xoa cac nut con va doi tuong
    }

#ifdef SFML_RENDERTARGET_HPP
    void draw(sf::RenderTarget& window) {
        window.draw(line);

        if (nodes[0] != nullptr) {
            nodes[0]->draw(window);
            nodes[1]->draw(window);
            nodes[2]->draw(window);
            nodes[3]->draw(window);
        }
    }
#endif

    // Phuong thuc chia nut hien tai thanh 4 nut con
    void split() {
        if (level == 0) { // Kiem tra neu cap do cua nut la 0, khong chia nua
            return;
        }

        T_size subWidth = bounds.width() / 2.f; // Chieu rong cua nut con
        T_size subHeight = bounds.height() / 2.f; // Chieu cao cua nut con
        T_size x = bounds.left; // Toa do x cua nut
        T_size y = bounds.top; // Toa do y cua nut

        // Khoi tao cac nut con (NW, NE, SW, SE)
        nodes[0] = new QuadTree<T_size, T_value>(math::Rect<T_size>(x, x + subWidth, y, y + subHeight), level - 1, max_objects); // NW
        nodes[1] = new QuadTree<T_size, T_value>(math::Rect<T_size>(x + subWidth, x + bounds.width(), y, y + subHeight), level - 1, max_objects); // NE
        nodes[2] = new QuadTree<T_size, T_value>(math::Rect<T_size>(x, x + subWidth, y + subHeight, y + bounds.height()), level - 1, max_objects); // SW
        nodes[3] = new QuadTree<T_size, T_value>(math::Rect<T_size>(x + subWidth, x + bounds.width(), y + subHeight, y + bounds.height()), level - 1, max_objects); // SE
    }

    // Phuong thuc xac dinh chi so cua doi tuong trong cac nut con
    int getIndex(const std::pair<math::Vector2<T_size>, T_value*>& object) const {
        if (
            nodes[0] == nullptr || 
            object.first.x < bounds.left || 
            object.first.x > bounds.right || 
            object.first.y < bounds.top || 
            object.first.y > bounds.bottom
        ) return -1;

        T_size verticalMidpoint = bounds.left + (bounds.right - bounds.left) / 2; // Diem giua theo chieu doc
        T_size horizontalMidpoint = bounds.top + (bounds.bottom - bounds.top) / 2; // Diem giua theo chieu ngang

        bool topQuadrant = (object.first.y < horizontalMidpoint); // Kiem tra neu doi tuong o nua tren
        bool rightQuadrant = (object.first.x < verticalMidpoint); // Kiem tra neu doi tuong o nua ben phai

        if (topQuadrant && rightQuadrant) return 0; // NW
        else if (topQuadrant && !rightQuadrant) return 1; // NE
        else if (!topQuadrant && rightQuadrant) return 2; // SW
        else return 3; // SE
    }

    // Phuong thuc chen doi tuong vao QuadTree
    void insert(std::pair<math::Vector2<T_size>, T_value*> object, math::Rect<T_size> size_object) {
        if (size_object.left < biggest.left)
        biggest.left = size_object.left;
        if (size_object.top < biggest.top)
        biggest.top = size_object.top;
        if (size_object.right > biggest.right)
        biggest.right = size_object.right;
        if (size_object.bottom > biggest.bottom)
        biggest.bottom = size_object.bottom;

        if (nodes[0] != nullptr) { // Neu nut hien tai da duoc chia
            int index = getIndex(object); // Lay chi so cua doi tuong
            if (index != -1 && nodes[index] != nullptr) {
                nodes[index]->insert(object, size_object); // Chen doi tuong vao nut con tuong ung
                return;
            }
        }

        sizes.push_back(size_object);
        objects.push_back(std::move(object)); // Chen doi tuong vao danh sach doi tuong cua nut hien tai

        if (objects.size() > max_objects && level > 0) { // Neu so luong doi tuong vuot qua gioi han va con cap do
            split(); // Chia nut hien tai thanh cac nut con

            auto it = objects.begin();
            while (it != objects.end()) {
                int index = getIndex(*it);
                if (index == -1 || nodes[index] == nullptr) ++it;
                else {
                    nodes[index]->insert(*it, sizes[it - objects.begin()]); // Chen doi tuong vao nut con tuong ung
                    sizes.erase(sizes.begin() + (it - objects.begin()));
                    it = objects.erase(it); // Xoa doi tuong khoi nut hien tai
                } 
            }
        }
    }

    // Phuong thuc truy van cac doi tuong gan doi tuong cho truoc
    void retrieve(std::vector<std::pair<math::Vector2<T_size>, T_value*>>& returnObjects, math::Vector2<T_size> position, math::Rect<T_size> size_object) {
        int index = getIndex({position, nullptr}); // Lay chi so cua doi tuong trong cac nut con

        switch (index) { // Truy van cac doi tuong trong nut con tuong ung
            case 0:
            {
                if (nodes[0] != nullptr) {
                    nodes[0]->retrieve(returnObjects, position, size_object);
                }
                break;
            }
            case 1:
            {
                if (nodes[0] != nullptr) {
                    if (position.x + size_object.left < nodes[0]->getBounds().right + nodes[0]->getBiggest().right) {
                        nodes[0]->retrieve(returnObjects, position, size_object);
                    }
                }
                if (nodes[1] != nullptr) {
                    nodes[1]->retrieve(returnObjects, position, size_object);
                }
                break;
            }
            case 2:
            {
                if (nodes[0] != nullptr) {
                    if (position.y + size_object.top < nodes[0]->getBounds().bottom + nodes[0]->getBiggest().bottom) {
                        nodes[0]->retrieve(returnObjects, position, size_object);
                    }
                }
                if (nodes[2] != nullptr) {
                    nodes[2]->retrieve(returnObjects, position, size_object);
                }
                break;
            }
            case 3: 
            {
                bool hasNotWN = true;
                if (position.x + size_object.left < nodes[0]->getBounds().right + nodes[0]->getBiggest().right) {
                    if (nodes[0] != nullptr) {
                        nodes[0]->retrieve(returnObjects, position, size_object);
                        hasNotWN = false;
                    }
                    if (nodes[2] != nullptr)
                    nodes[1]->retrieve(returnObjects, position, size_object);
                }
                if (position.y + size_object.top < nodes[0]->getBounds().bottom + nodes[0]->getBiggest().bottom) {
                    if (nodes[0] != nullptr && hasNotWN)
                    nodes[0]->retrieve(returnObjects, position, size_object);
                    if (nodes[1] != nullptr)
                    nodes[1]->retrieve(returnObjects, position, size_object);
                }
                if (nodes[3] != nullptr) {
                    nodes[3]->retrieve(returnObjects, position, size_object);
                }
                break;
            }
            default:
            {
                if (nodes[0] != nullptr) {
                    nodes[0]->retrieve(returnObjects, position, size_object);
                }
                if (nodes[1] != nullptr) {
                    nodes[1]->retrieve(returnObjects, position, size_object);
                }
                if (nodes[2] != nullptr) {
                    nodes[2]->retrieve(returnObjects, position, size_object);
                }
                if (nodes[3] != nullptr) {
                    nodes[3]->retrieve(returnObjects, position, size_object);
                }
                break;
            }
        }

        // Them cac doi tuong trong nut hien tai vao danh sach tra ve
        returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
    }

    // Phuong thuc xoa tat ca cac doi tuong va cac nut con trong QuadTree
    void clear() {
        for (int i = 0; i < 4; i++) {
            if (nodes[i] != nullptr) {
                nodes[i]->clear(); // Goi ham clear cua nut con
                delete nodes[i]; // Giai phong bo nho cua nut con
                nodes[i] = nullptr;
            }
        }
        objects.clear(); // Xoa tat ca doi tuong trong nut hien tai
    }

    const math::Rect<T_size>& getBounds() const { return bounds; }
    const math::Rect<T_size>& getBiggest() const { return biggest; }
};

} // namespace algorithm
} // namespace BM

#endif
