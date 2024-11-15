#ifndef __TOOL_OBJECT_SCALE_HPP__
#define __TOOL_OBJECT_SCALE_HPP__

namespace ng {
    class Scale : public SampleObject {
        private:
            mutable sf::VertexArray line {sf::Lines, 2};
            mutable sf::CircleShape head;

            mutable float maxRatio = 100.f;

            mutable float view = 0.f;

            void redraw() const override {
                renderTexture.draw(line);
                renderTexture.draw(head);
            }

            void handleSetSize() const override {
                if (renderTexture.getSize() != sf::Vector2u(0, 0) && line[0].position.x != line[1].position.x) {
                    setView(view / (line[1].position.x - line[0].position.x) * renderTexture.getSize().x);
                }
                line[0].position = {0.f, renderTexture.getSize().y / 2.f};
                line[1].position = {(float) renderTexture.getSize().x, renderTexture.getSize().y / 2.f};
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
                    (unsigned int) json["themes"][theme]["color-semi-slider-1"][0],
                    (unsigned int) json["themes"][theme]["color-semi-slider-1"][1],
                    (unsigned int) json["themes"][theme]["color-semi-slider-1"][2],
                    (unsigned int) json["themes"][theme]["color-semi-slider-1"][3]
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
                    (unsigned int) json["themes"][theme]["color-outline-line"][0],
                    (unsigned int) json["themes"][theme]["color-outline-line"][1],
                    (unsigned int) json["themes"][theme]["color-outline-line"][2],
                    (unsigned int) json["themes"][theme]["color-outline-line"][3]
                ));

                setOutlineThinkness((float) json["themes"][theme]["thinkness-outline-border-in"]);
            }
        public:
            Scale() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setHeadRadius((float) json["other"]["scale"]["radius"]);

                setSize(sf::Vector2u((unsigned int) json["size"]["slider"][0], (unsigned int) json["size"]["slider"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["scale"][0], (float) json["position"]["scale"][1]));

                setView((float) json["other"]["scale"]["view"]);
            }

            void setHeadColor(const sf::Color& color) const {
                needsRedraw = true;
                head.setFillColor(color);
            }

            void setHeadRadius(float radius) const {
                needsRedraw = true;
                head.setRadius(radius);
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

            void setLineColor(const sf::Color& color) const {
                needsRedraw = true;
                line[0].color = color;
                line[1].color = color;
            }

            void setView(float newView) const {
                if (newView < 0.f) {
                    newView = 0.f;
                }
                else if (newView > renderTexture.getSize().x) {
                    newView = renderTexture.getSize().x;
                }

                needsRedraw = true;
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
                return head.getRadius(); 
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
