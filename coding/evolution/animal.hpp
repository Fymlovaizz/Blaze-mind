
// Lop animal - Doi tuong dong vat, ke thua tu lop Entity
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
