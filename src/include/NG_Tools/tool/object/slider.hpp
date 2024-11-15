#ifndef __TOOL_OBJECT_SLIDER_HPP__
#define __TOOL_OBJECT_SLIDER_HPP__

namespace ng {
    class Slider : public SampleObject {
        private:
            mutable sf::VertexArray line {sf::Lines, 5};
            mutable sf::RectangleShape purdah;
            mutable sf::RectangleShape head;

            mutable float maxRatio = 100.f;
            mutable float radius;

            mutable float view = 0.f;
            mutable bool reset = true;

            void drawPurdah() const {
                if (view - renderTexture.getSize().y + radius > 0.f) {
                    purdah.setSize({view - renderTexture.getSize().y + radius, radius * 2.f});
                }
                else {
                    purdah.setSize({0.1f, radius * 2.f});
                }
                purdah.setPosition(renderTexture.getSize().y / 2.f - radius, renderTexture.getSize().y / 2.f - radius);
            }

            void redraw() const override {
                if (reset) {
                    reset = false;
                    drawPurdah();
                }

                renderTexture.draw(line);
                renderTexture.draw(purdah);
                renderTexture.draw(head);
            }

            void handleSetSize() const override {
                if (renderTexture.getSize() != sf::Vector2u(0, 0)) {
                    line[0].position = {0.f, 0.f};
                    line[1].position = {(float) renderTexture.getSize().x - 1.f, 0.f};
                    line[2].position = {(float) renderTexture.getSize().x - 1.f, (float) renderTexture.getSize().y - 1.f};
                    line[3].position = {1.f, (float) renderTexture.getSize().y - 1.f};
                    line[4].position = {1.f, 0.f};
                }
            }

            void handlePress() const override {
                setView((window->getMousePosition().x - sprite.getPosition().x - sprite.getOrigin().x) / sprite.getScale().x);
            }

            void handleDrag() const override {
                setView(view + window->getMouseDeltapos().x * sprite.getScale().x);
            }

            void handleScroll() const override {
                setView(view + window->getEvent().mouseWheelScroll.delta * 5.f);
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-semi-slider-2"][0],
                    (unsigned int) json["themes"][theme]["color-semi-slider-2"][1],
                    (unsigned int) json["themes"][theme]["color-semi-slider-2"][2],
                    (unsigned int) json["themes"][theme]["color-semi-slider-2"][3]
                ));

                setHeadColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-main"][0],
                    (unsigned int) json["themes"][theme]["color-core-main"][1],
                    (unsigned int) json["themes"][theme]["color-core-main"][2],
                    (unsigned int) json["themes"][theme]["color-core-main"][3]
                ));

                setOutlineColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][0],
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][1],
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][2],
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][3]
                ));

                setLineColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][0],
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][1],
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][2],
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][3]
                ));

                setPurdahColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-secondary"][0],
                    (unsigned int) json["themes"][theme]["color-core-secondary"][1],
                    (unsigned int) json["themes"][theme]["color-core-secondary"][2],
                    (unsigned int) json["themes"][theme]["color-core-secondary"][3]
                ));

                setOutlineThinkness((float) json["themes"][theme]["thinkness-outline-border-in"]);
            }
        public:
            Slider() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["slider"][0], (unsigned int) json["size"]["slider"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["slider"][0], (float) json["position"]["slider"][1]));

                setHeadRadius((float) json["other"]["slider"]["radius"]);

                setView((float) json["other"]["slider"]["view"]);
            }

            void setHeadColor(const sf::Color& color) const {
                needsRedraw = true;
                head.setFillColor(color);
            }

            void setHeadRadius(float radius) const {
                needsRedraw = true;
                reset = true;
                if (radius <= 0.f) {
                    std::cerr << "Warning: Slider head radius must be positive!" << std::endl;
                    return;
                }
                else if (radius > renderTexture.getSize().y / 2.f) {
                    std::cerr   << "Warning: Slider head radius is too big!\n\t- Input: '" << radius 
                                << "'.\n\t- Radius will set to '" << renderTexture.getSize().y / 2.f << "'!" << std::endl;
                    radius = renderTexture.getSize().y / 2.f;
                }
                this->radius = radius;
                head.setSize({radius * 2.f, radius * 2.f});
                head.setOrigin(radius, radius);
            }

            void setOutlineColor(const sf::Color& color) const {
                needsRedraw = true;
                head.setOutlineColor(color);
            }

            void setOutlineThinkness(float thickness) const {
                needsRedraw = true;
                head.setOutlineThickness(thickness);
            }

            void setPurdahColor(const sf::Color& color) const {
                needsRedraw = true;
                purdah.setFillColor(color);
            }

            void setLineColor(const sf::Color& color) const {
                needsRedraw = true;
                line[0].color = color;
                line[1].color = color;
                line[2].color = color;
                line[3].color = color;
                line[4].color = color;
            }

            void setView(float newView) const {
                if (newView < 0.f) {
                    newView = 0.f;
                }
                else if (newView > renderTexture.getSize().x) {
                    newView = renderTexture.getSize().x;
                }

                needsRedraw = true;
                reset = true;
                view = newView;
                head.setPosition({
                    view,
                    renderTexture.getSize().y / 2.f
                });

            }

            void setRatio(float ratio) const {
                if (ratio < 0.f) {
                    ratio = 0.f;
                }
                else if (ratio > maxRatio) {
                    ratio = maxRatio;
                }

                setView(ratio / maxRatio * renderTexture.getSize().x);
            }

            void setMaxRatio(float newMaxRatio) const {
                if (newMaxRatio < 0.f) {
                    newMaxRatio = 0.f;
                }

                maxRatio = newMaxRatio;
            }

            const sf::Color& getHeadColor() const {
                return head.getFillColor();
            }

            float getHeadRadius() const {
                return radius;
            }

            const sf::Color& getOutlineColor() const {
                return head.getOutlineColor();
            }

            float getOutlineThinkness() const {
                return head.getOutlineThickness();
            }

            const sf::Color& getLineColor() const {
                return line[0].color;
            }

            float getView() const {
                return view;
            }

            float getRatio() const {
                return (float) view / (float) renderTexture.getSize().x * maxRatio;
            }

            float getMaxRatio() const {
                return maxRatio;
            }
    };
}

#endif
