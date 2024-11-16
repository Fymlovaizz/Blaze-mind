bool draw_vision = true;
bool draw_target = true;

// Lop Entity - Lop co so cho cac thuc the trong tro choi
class Entity {
protected:
    BM::math::Vector2f position; // Vi tri cua thuc the
    float rotation;              // Goc xoay cua thuc the

    float health = 50;           // Suc khoe mac dinh
    float age = 35;              // Tuoi tho mac dinh

    float radius;                // Ban kinh Collision
    sf::ConvexShape shape;       // Hinh dang cua thuc the

    // Phuong thuc thiet lap ban dau (co the duoc ghi de)
    virtual void setup() {};
public:
    // Destructor ao
    virtual ~Entity() = default;

    // Phuong thuc cap nhat (co the duoc ghi de)
    virtual int update(float dt) { return 0; }

    // Phuong thuc ve thuc the len cua so
    virtual void draw(sf::RenderTarget& window) { window.draw(this->shape); }

    // Dat vi tri cho thuc the
    virtual void setPosition(BM::math::Vector2f position) {
        this->position = position;
        this->shape.setPosition(this->position.sf());
    }

    // Dat goc xoay cho thuc the
    void setRotation(float rotation) {
        this->rotation = rotation;
        this->shape.setRotation(this->rotation);
        if (this->rotation > 360) this->rotation -= 360;
        if (this->rotation < 0) this->rotation += 360;
    }

    // Lay vi tri hien tai cua thuc the
    BM::math::Vector2f getPosition() { return this->position; }
    BM::math::Vector2f& getPositionRef() { return this->position; }

    // Lay goc xoay hien tai cua thuc the
    float getRotation() { return this->rotation; }

    // Lay suc khoe hien tai cua thuc the
    float getHealth() { return this->health; }

    // Lay tuoi tho hien tai cua thuc the
    float getAge() { return this->age; }

    // Lay ban kinh Collision
    float getRadius() { return this->radius; }
};

