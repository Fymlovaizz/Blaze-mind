#ifndef __TOOL_OBJECT_STEPPER_HPP__
#define __TOOL_OBJECT_STEPPER_HPP__

namespace ng {
    class Stepper : public SampleObject {
        private:
            mutable sf::VertexArray line {sf::Lines, 2};
            mutable sf::RectangleShape head;

            mutable std::size_t step;
            mutable std::size_t segment = 2.f;
            mutable float space;
            mutable float view = 0.f;

            void setView(float newView) const {
                if (newView < 0.f) {
                    newView = 0.f;
                }
                else if (newView > renderTexture.getSize().x) {
                    newView = renderTexture.getSize().x;
                }

                segment = (std::size_t) ((newView - space / 2.f) / space + 1.f);

                needsRedraw = true;
                view = newView;
                head.setPosition({
                    view,
                    renderTexture.getSize().y / 2.f
                });
            }

            void redraw() const override {
                renderTexture.draw(line);
                renderTexture.draw(head);
            }

            void handleSetSize() const override {
                space = renderTexture.getSize().x / (segment - 1.f);
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

            void handleRelease() const override {
                needsRedraw = true;
                view = segment * space;
                head.setPosition({
                    view,
                    renderTexture.getSize().y / 2.f
                });
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
            Stepper() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setHeadSize(sf::Vector2f((float) json["other"]["stepper"]["size"][0], (float) json["other"]["stepper"]["size"][1]));

                setSize(sf::Vector2u((unsigned int) json["size"]["slider"][0], (unsigned int) json["size"]["slider"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["stepper"][0], (float) json["position"]["stepper"][1]));

                setSegment((std::size_t) json["other"]["stepper"]["segment"]);
                setStep((std::size_t) json["other"]["stepper"]["step"]);
            }

            void setHeadColor(const sf::Color& color) const {
                needsRedraw = true;
                head.setFillColor(color);
            }

            void setHeadSize(const sf::Vector2f& size) const {
                needsRedraw = true;
                head.setSize(size);
                head.setOrigin(size / 2.f);
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

            void setSegment(std::size_t newSegment) const {
                if (newSegment < 2) {
                    std::cerr << "Warning: Segment must be greater than 1.\n\t- Input: " << 
                    newSegment << "\n\t- Segment will set to 2." << std::endl;                    
                    newSegment = 2;
                }
                needsRedraw = true;
                segment = newSegment;
                space = renderTexture.getSize().x / (segment - 1.f);
            }

            void setStep(std::size_t newStep) const {
                if (newStep >= segment) {
                    std::cerr << "Warning: Step must be less than segment.\n\t- Input: " << 
                    newStep << "\n\t- Step will set to segment - 1 (" << segment - 1 << ")." << std::endl;
                    newStep = segment - 1;
                }
                needsRedraw = true;
                step = newStep;
                view = step * space;
                head.setPosition({
                    view,
                    renderTexture.getSize().y / 2.f
                });
            }
            
            const sf::Color& getHeadColor() const { 
                return head.getFillColor(); 
            }

            const sf::Vector2f& getHeadSize() const { 
                return head.getSize(); 
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

            const std::size_t& getSegment() const {
                return segment;
            }

            const std::size_t& getStep() const {
                return step;
            }
    };
}

#endif
