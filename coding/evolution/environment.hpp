BM::math::Vector2f size_quadtree = BM::math::Vector2f(1920.f, 1920.f) * 100.f;
BM::math::Vector2f region_spam_animal = BM::math::Vector2f(1920.f, 1920.f) * 3.f;
BM::math::Vector2f region_spam_plant = BM::math::Vector2f(1920.f, 1920.f) * 10.f;
int number_animals = 10;
int number_plants = 12500;
bool show_quadtree_animal = false;
bool show_quadtree_resource = true;
float time_more = 10.f;

namespace ng {

// SampleObject la 1 doi tuong duoc thiet ke cho viec su dung cac widget.
// Muon tim hieu them co the vo SampleObject de xem thu code cua no.
// Moi ham handle duoc ke thua tu SampleObject se duoc goi tu cua so mot cach tu dong.
class Bio : public SampleObject {
private:
    mutable sf::View view; // Che do xem cua cua so
    mutable float zoomFactor = 1.0f; // Ty le thu phong
    mutable std::vector<Animal> animals; // Danh sach cac dong vat
    mutable std::vector<Resource> resources; // Danh sach cac tai nguyen
    mutable bool idn = false;

    // Cau truc cay tu phan de quan ly cac thuc the
    mutable BM::algorithm::QuadTree<float, Animal> quadtree_animal = {BM::math::FloatRect(-size_quadtree.x, size_quadtree.x, -size_quadtree.y, size_quadtree.y), 32, 4};
    mutable BM::algorithm::QuadTree<float, Resource> quadtree_resource = {BM::math::FloatRect(-size_quadtree.x, size_quadtree.x, -size_quadtree.y, size_quadtree.y), 32, 10};

    sf::Font font; // Font chu
    mutable sf::Text text; // Chuoi thong bao

    template <typename T>
    void checkCollisions(BM::algorithm::QuadTree<float, T>& quadtree) const {
        std::vector<Animal> virtual_animals;
        for (auto& animal : animals) {
            std::vector<T*> nearbyEntities;
            quadtree.retrieve(nearbyEntities, animal.getPosition(), {-animal.getRadius(), animal.getRadius(), -animal.getRadius(), animal.getRadius()});
            for (auto& other : nearbyEntities) {
                if (other != nullptr) {
                    if (std::is_same<T, Animal>::value) {
                        // Animal* otherAnimal = dynamic_cast<Animal*>(other);
                        // if (&animal == otherAnimal) continue;
                    }
                    else {
                        try {
                            Resource* otherResource = dynamic_cast<Resource*>(other);
                            if (otherResource != nullptr) {
                                BM::math::Vector2f delta = {otherResource->getPosition() - animal.getPosition()};
                                float distance = delta.length_squared();
                                float radius_sum = (animal.getRadius() + otherResource->getRadius()) * (animal.getRadius() + otherResource->getRadius());
                                float vision = distance + radius_sum - 2 * sqrt(distance) * sqrt(radius_sum);
                                animal.putTargetResource(delta, vision);
                                if (distance < radius_sum) {
                                    otherResource->setHealth(0);
                                    if (animal.appendEnergy(otherResource->getEnergy())) {
                                        virtual_animals.emplace_back(Animal(animal.getPosition(), -animal.getRotation()));
                                        virtual_animals.back().setColor(BM::math::getRandomGeneticColor(animal.getColor()));
                                        virtual_animals.back().setIndex(40.f, 80.f, 100.f);
                                        virtual_animals.back().setVision(BM::math::max(10000.f, animal.getVision() + BM::math::getRandomFloat(-900.f, 900.f)));
                                        virtual_animals.back().setVelocity(BM::math::max(50.f, animal.getVelocity() + BM::math::getRandomFloat(-5.f, 5.f)));
                                    }
                                }
                            }
                        }
                        catch (const std::exception& e) {
                            std::cout << "Caught exception: " << e.what() << std::endl;
                        }
                        catch (...) {
                            std::cout << "Caught unknown exception" << std::endl;
                        }
                    }
                }
            }
        }
        while (!virtual_animals.empty()) {
            animals.push_back(virtual_animals.back());
            virtual_animals.pop_back();
        }
    }

    void processing_resources() const {
        quadtree_resource.clear(); // Xoa cac thuc the cu khoi quadtree
        std::vector<Resource> virtual_resources;
        for (auto it = resources.begin(); it != resources.end(); ) {
            int update = it->update(window->getDeltatime() * time_more);
            if (update == -1) {
                it = resources.erase(it); // Xoa tai nguyen neu het suc khoe
                continue;
            }
            if (update == 1) { // Cap nhat tai nguyen
                // Kiem tra neu can tao tai nguyen moi
                if (it->getType() == 0) {
                    virtual_resources.push_back(Resource(
                        it->getPosition() + BM::math::Vector2f(
                            BM::math::getRandomInt(-it->getRadius() * 4.5f, +it->getRadius() * 4.5f),
                            BM::math::getRandomInt(-it->getRadius() * 4.5f, +it->getRadius() * 4.5f)
                        ),
                        0
                    ));
                    virtual_resources.back().setType(0, BM::math::max(5.f, it->getRadius() * BM::math::getRandomFloat(0.8, 1.25)));
                }
            }
            if (it->getHealth() <= 0) {
                it = resources.erase(it); // Xoa tai nguyen neu het suc khoe
                continue;
            }
            it->draw(renderTexture); // Ve tai nguyen len cua so
            // Chen tai nguyen vao quadtree
            quadtree_resource.insert({BM::math::Vector2f(it->getPosition()), &*it}, {-it->getRadius(), it->getRadius(), -it->getRadius(), it->getRadius()});
            ++it;
        }
        while (!virtual_resources.empty()) {
            resources.push_back(virtual_resources.back()); // Them tai nguyen moi vao danh sach
            virtual_resources.pop_back();
        }        
    }

    void processing_animals() const {
        quadtree_animal.clear(); // Xoa cac thuc the cu khoi quadtree
        for (auto it = animals.begin(); it != animals.end(); ) {
            if (it->update(window->getDeltatime() * time_more) == -1 || it->getHealth() <= 0) {
                resources.emplace_back(Resource(it->getPosition(), 0));
                resources.back().setType(1, it->getRadius() * 0.5f);
                it = animals.erase(it); // Xoa dong vat neu het tuoi tho hoac suc khoe
                continue;
            }
            it->draw(renderTexture); // Ve dong vat len cua so
            // Chen dong vat vao quadtree
            quadtree_animal.insert({BM::math::Vector2f(it->getPosition()), &*it}, {-it->getRadius(), it->getRadius(), -it->getRadius(), it->getRadius()});
            ++it;
        }
    }

    void handleShift() const override {
        if (window->getEvent().key.code == sf::Keyboard::Q) show_quadtree_animal = !show_quadtree_animal;
        if (window->getEvent().key.code == sf::Keyboard::W) show_quadtree_resource = !show_quadtree_resource;
    }

    void handleScroll() const override {
        if (idn) {
            idn = false;
            return;
        }
        if (window->getEvent().mouseWheelScroll.delta > 0) {
            zoomFactor *= 1.05f;  // Phong to
        }
        else {
            zoomFactor /= 1.05f;  // Thu nho
        }
        view.setSize(1920.f / zoomFactor, 1080.f / zoomFactor);
    }

    void handleDrag() const override {
        idn = true;
        view.move(-window->getMouseDeltapos() / zoomFactor);
    }

    void update() const override {
        needsRedraw = true;
    }

    void redraw() const override {
        renderTexture.setView(view); // Dat view cho cua so

        processing_resources();
        processing_animals();

        if (show_quadtree_resource) quadtree_resource.draw(renderTexture);
        if (show_quadtree_animal) quadtree_animal.draw(renderTexture);

        checkCollisions(quadtree_resource);
        checkCollisions(quadtree_animal);

        text.setString(
            "Number of resources: " + std::to_string(resources.size()) + 
            "\nNumber of animals: " + std::to_string(animals.size())
        );
        renderTexture.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));
        renderTexture.draw(text);
    }
public:
    Bio() {
        view = sf::View(sf::FloatRect(0, 0, 1920, 1080));
        font.loadFromFile("./resources/fonts/arial.ttf");
        text.setFont(font);
        text.setCharacterSize(12);
        text.setPosition(10.f, 24.f);

        setSize(sf::Vector2u(1920, 1080));
        setPosition(sf::Vector2f(0.f, 0.f));

        for (int i = 0; i < number_animals; i++) {
            animals.push_back(Animal(
                sf::Vector2f(BM::math::getRandomInt(-region_spam_animal.x, region_spam_animal.x), BM::math::getRandomInt(-region_spam_animal.y, region_spam_animal.y)),
                BM::math::getRandomInt(0, 360)
            ));
            animals.back().setIndex(40.f, 80.f, 100.f);
            animals.back().setColor(sf::Color(180, 40, 40, 180));
        }

        for (int i = 0; i < number_plants; i++) {
            resources.push_back(Resource(
                sf::Vector2f(BM::math::getRandomInt(-region_spam_plant.x, region_spam_plant.x), BM::math::getRandomInt(-region_spam_plant.y, region_spam_plant.y)), 
                0
            ));
        }

        sprite.setPosition(0.f, 0.f);
        sprite.setTexture(renderTexture.getTexture(), true);

        setColor(sf::Color(10, 10, 20));
    }
};

}