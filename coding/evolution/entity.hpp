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

// Lop Resource - Doi tuong tai nguyen, ke thua tu lop Entity
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

// Lop Animal - Doi tuong dong vat, ke thua tu lop Entity
class Animal : public Entity {
private:
    std::string name; // Ten cua dong vat

    BM::math::Vector2f direction; // Huong di chuyen
    BM::math::Vector2f velocity; // Vi tri tam thoi
    float angular_velocity;      // Toc do quay
    float wait = 0;              // Thoi gian cho doi
    float travel = 0;            // Thoi gian di chuyen
    bool is_moving = false;      // Co di chuyen hay khong
    bool has_move = false;
    float time = 0;              // Thoi gian hien tai

    float attack;                // Suc tan cong

    float energy;                // Nang luong hien tai
    float max_energy;            // Nang luong toi da

    float distanceWithFootNearest = FLOAT_MAX;
    float angleWithFootNearest = FLOAT_MAX;
    float vision;

    sf::CircleShape visionRegion;

    sf::VertexArray line = sf::VertexArray(sf::Lines, 2);
public:
    // Constructor
    Animal(BM::math::Vector2f position, float rotation) {
        setPosition(position);
        setRotation(rotation);

        this->shape.setOutlineColor(sf::Color::Black);
        this->shape.setOutlineThickness(3);

        this->direction = BM::math::Vector2f(0, 0);
        this->health = 100;
        this->attack = 10;
        this->age = BM::math::getRandomFloat(90.f, 150.f);
        this->max_energy = 120;
        this->energy = BM::math::getRandomFloat(this->max_energy / 2.f, this->max_energy);


        this->wait = BM::math::getRandomFloat(0.5f, 1.0f);
        this->travel = BM::math::getRandomFloat(1.0f, 1.5f);

        this->velocity = {(float) BM::math::getRandomFloat(240.f, 360.f)};
        this->angular_velocity = BM::math::getRandomFloat(40.f, 45.f) * (BM::math::getRandomBool() ? 1 : -1);

        this->vision = BM::math::getRandomFloat(10000.f, 40000.f);
        this->visionRegion.setFillColor(sf::Color(255, 255, 255, 10));
        this->visionRegion.setRadius(sqrt(this->vision));
        this->visionRegion.setOrigin(sqrt(this->vision), sqrt(this->vision));
    }

    // Phuong thuc cap nhat, ghi de tu lop Entity
    int update(float dt) override {
        this->age -= dt; // Giam tuoi tho theo thoi gian troi qua
        this->energy -= dt;
        if (this->age < 0) return -1; // Tra ve -1 neu tuoi tho <= 0, nghia la dong vat da chet
        if (this->energy < 0) return -1;

        if (this->angleWithFootNearest != FLOAT_MAX && !is_moving) {
            // Xử lý việc xoay hướng về mục tiêu
            float angle_diff = this->angleWithFootNearest - this->rotation;

            // Đưa góc về phạm vi [-180, 180] để dễ dàng tính toán
            while (angle_diff > 180.0f) angle_diff -= 360.0f;
            while (angle_diff < -180.0f) angle_diff += 360.0f;

            // Tính toán hướng quay
            float rotate_direction = (angle_diff > 0) ? 1.0f : -1.0f;

            // Thay đổi góc quay theo vận tốc góc
            float rotation_step = BM::math::abs(this->angular_velocity) * dt * rotate_direction;

            // Nếu bước quay lớn hơn góc cần quay, thì trực tiếp đặt góc quay khớp mục tiêu
            if (BM::math::abs(rotation_step) >= BM::math::abs(angle_diff)) {
                setRotation(this->angleWithFootNearest);
                this->angleWithFootNearest = FLOAT_MAX;
                is_moving = true;
            }
            else {
                this->rotation += rotation_step;
                this->direction = BM::math::Vector2f(std::sin(this->rotation / 180 * M_PI), -std::cos(this->rotation / 180 * M_PI));
                this->shape.setRotation(this->rotation);
            }
        }
        else {
            // Kiem tra neu khong co muc tieu di chuyen
            if (!is_moving) {
                if (this->distanceWithFootNearest != FLOAT_MAX) this->distanceWithFootNearest = FLOAT_MAX;
                rotate(dt); // Xoay dong vat
                if (BM::math::getRandomBool(dt)) { // Ngau nhien thay doi muc tieu di chuyen
                    is_moving = true;
                    this->angular_velocity *= -1; // Dao nguoc huong xoay
                }
            } else {
                if (has_move) {
                    move(dt); // Di chuyen dong vat
                    this->time -= dt;
                    if (this->time < 0) {
                        this->time = wait;
                        is_moving = false;
                        has_move = false;
                        this->distanceWithFootNearest = FLOAT_MAX;
                    }
                }
                else {
                    this->time -= dt;
                    if (this->time < 0) {
                        this->time = travel;
                        has_move = true;
                    }
                }
            }

        }

        return 0; // Tra ve 0 neu khong co thay doi dac biet
    }

    void draw(sf::RenderTarget& window) override {
        if (draw_vision) window.draw(this->visionRegion);
        window.draw(this->shape); 

        if (draw_target && this->distanceWithFootNearest != FLOAT_MAX) {
            line[0].color = sf::Color::Blue;
            line[1].color = sf::Color::Blue;
            line[0].position = shape.getPosition();

            window.draw(this->line);
        }
    }

    void setPosition(BM::math::Vector2f position) override {
        this->position = position;
        this->shape.setPosition(this->position.sf());
        this->visionRegion.setPosition(this->position.sf());
    }

    // Thiet lap chi so kich thuoc cua dong vat
    void setIndex(float width, float height, float virtual_height) {
        this->radius = BM::math::max(width, height) / 2.f;

        this->shape.setPointCount(5);
        this->shape.setPoint(0, {-width * height / virtual_height, height - virtual_height});
        this->shape.setPoint(1, {-width / 2, -height / 2.f});
        this->shape.setPoint(2, {+width / 2, -height / 2.f});
        this->shape.setPoint(3, {+width * height / virtual_height, height - virtual_height});
        this->shape.setPoint(4, {0, height / 2.f});
    }

    // Phuong thuc di chuyen
    void move(float dt) {
        this->position += this->direction * this->velocity * dt;
        this->shape.setPosition(this->position.sf());
        this->visionRegion.setPosition(this->position.sf());
    }

    // Phuong thuc xoay
    void rotate(float dt) {
        this->rotation += this->angular_velocity * dt;
        this->direction = BM::math::Vector2f(std::sin(this->rotation / 180 * M_PI), -std::cos(this->rotation / 180 * M_PI));
        this->shape.setRotation(this->rotation);
        if (this->rotation > 360) this->rotation -= 360;
        if (this->rotation < 0) this->rotation += 360;
    }

    // Dat mau cho hinh dang cua dong vat
    void setColor(sf::Color color) {
        this->shape.setFillColor(color); 
    }

    sf::Color getColor() { return this->shape.getFillColor(); }

    int appendEnergy(float energy) {
        this->energy += energy;
        if (this->energy > this->max_energy) {
            this->energy -= this->max_energy * 0.4f;
            return 1;
        }
        return 0;
    }

    void putTargetResource(BM::math::Vector2f delta, float distance) {
        if (distance < this->vision && distance < this->distanceWithFootNearest) {
            this->distanceWithFootNearest = distance;
            this->angleWithFootNearest = std::atan2(delta.x, -delta.y) * 180 / M_PI;
            line[1].position = delta.sf() + this->position.sf();
        }
    }

    float getEnergy() { return this->energy; }

    void setVision(float vision) { 
        this->vision = vision;
        this->visionRegion.setRadius(sqrt(vision));
        this->visionRegion.setOrigin(sqrt(vision), sqrt(vision));
    }

    float getVision() { return this->vision; }

    void setVelocity(float velocity_new) { this->velocity = {velocity_new, velocity_new}; }

    float getVelocity() { return this->velocity.x; }
};
