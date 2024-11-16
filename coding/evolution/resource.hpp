
class Resource : public Entity {
private:
    int type; // Loai tai nguyen
    float time = 0;
    BM::math::Vector2f velocity;

    void move_float(float dt) {
        this->position += this->velocity * dt;
        this->velocity *= 0.8;
        if (this->velocity.length_squared() < 1) this->velocity = {};
        this->shape.setPosition(this->position.sf());
    }
public:
    // Constructor
    Resource(BM::math::Vector2f position, float rotation) {
        setPosition(position);
        setRotation(rotation);

        setType(0, BM::math::getRandomFloat(50.f, 100.f));
    }

    // Phuong thuc cap nhat, ghi de tu lop Entity
    int update(float dt) override {
        this->age -= dt; // Giam tuoi tho theo thoi gian troi qua
        this->time -= dt;

        if (this->age < 0) return -1; // Tra ve -1 neu tuoi tho duoi 0

        move_float(dt); // Di chuyen vat
        if (this->time < 0) { // Kiem tra neu tuoi tho duoi 0
            if (this->type == 0) { // Neu loai tai nguyen la 0
                this->time += BM::math::getRandomFloat(60.f, 90.f); // Dat lai tuoi tho ngau nhien
                return 1; // Tra ve 1 neu can tao tai nguyen moi
            }
        }

        return 0; // Tra ve 0 neu khong co thay doi dac biet
    }

    // Dat loai va ban kinh cho tai nguyen
    void setType(int type, float radius) {
        this->type = type;
        this->radius = radius;

        switch (this->type) {
            case 0:
                this->age = BM::math::getRandomFloat(240.f, 320.f);
                this->shape.setFillColor(sf::Color(50, 80, 50));
                this->shape.setPointCount(8);    // Phan dat so dinh

                this->shape.setPoint(0, {0, +radius});  // Gia tri toa do tai moi dinh
                this->shape.setPoint(1, {+radius * (float) M_SQRT1_2, +radius * (float) M_SQRT1_2});
                this->shape.setPoint(2, {+radius, 0});
                this->shape.setPoint(3, {+radius * (float) M_SQRT1_2, -radius * (float) M_SQRT1_2});
                this->shape.setPoint(4, {0, -radius});
                this->shape.setPoint(5, {-radius * (float) M_SQRT1_2, -radius * (float) M_SQRT1_2});
                this->shape.setPoint(6, {-radius, 0});
                this->shape.setPoint(7, {-radius * (float) M_SQRT1_2, +radius * (float) M_SQRT1_2});

                this->health = radius * radius * M_PI;         // Co gia tri bang the tich tao. O day gia dinh vat co hinh tron
                this->time = BM::math::getRandomFloat(30.f, 200.f);   // Thoi gian ngau nhien de sinh san
                break;
            case 1:
                this->age = BM::math::getRandomFloat(50.f, 90.f);
                this->shape.setFillColor(sf::Color(150, 50, 50));
                this->shape.setPointCount(8);    // Phan dat so dinh

                this->shape.setPoint(0, {0, +radius});  // Gia tri toa do tai moi dinh
                this->shape.setPoint(1, {+radius * (float) M_SQRT1_2, +radius * (float) M_SQRT1_2});
                this->shape.setPoint(2, {+radius, 0});
                this->shape.setPoint(3, {+radius * (float) M_SQRT1_2, -radius * (float) M_SQRT1_2});
                this->shape.setPoint(4, {0, -radius});
                this->shape.setPoint(5, {-radius * (float) M_SQRT1_2, -radius * (float) M_SQRT1_2});
                this->shape.setPoint(6, {-radius, 0});
                this->shape.setPoint(7, {-radius * (float) M_SQRT1_2, +radius * (float) M_SQRT1_2});

                this->health = radius * radius * M_PI;                
            default:
                break;
        }
    }

    // Lay loai tai nguyen
    int getType() { return this->type; }

    // Lay toc do di chuyen
    BM::math::Vector2f getVelocity() { return this->velocity; }
    virtual BM::math::Vector2f& getVelocityRef() { return this->velocity; }

    float getEnergy() { 
        switch (this->type) {
            case 0:
                return this->radius * this->radius * M_PI / 2000.f;
            default:
                return this->radius * this->radius * M_PI / 200.f;
        }
    }

    void setHealth(float health) { this->health = health; }
};
