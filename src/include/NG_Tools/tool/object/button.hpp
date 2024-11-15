#ifndef __TOOL_OBJECT_BUTTON_HPP__
#define __TOOL_OBJECT_BUTTON_HPP__

namespace ng {
    class Button : public SampleObject {
        private:
            mutable sf::RectangleShape purdah;

            mutable float time = 0.f;
            mutable float delayTime = 0.05f;
            mutable bool status = false;
            mutable bool inSide = false;

            mutable unsigned char enter = 30;
            mutable unsigned char click = 90;

            mutable std::function<void()> func;

            void update() const override {
                if (time > 0.f) {
                    time -= window->getDeltatime();
                    if (time <= 0.f) {
                        needsRedraw = true;
                        status = false;
                        purdah.setFillColor(sf::Color(0, 0, 0, enter));
                    }
                }
            }

            void redraw() const override {
                if (inSide || status) {
                    renderTexture.draw(purdah);
                }
            }

            void handleEnter() const override {
                needsRedraw = true;
                inSide = true;
            }

            void handleLeave() const override {
                inSide = false;
                if (time <= 0.f) {
                    needsRedraw = true;
                }
            }

            void handlePress() const override {
                if (!status) {
                    needsRedraw = true;
                    status = true;

                    purdah.setFillColor(sf::Color(0, 0, 0, click));
                }
            }

            void handleRelease() const override {
                if (status) {
                    needsRedraw = true;
                    status = false;

                    purdah.setFillColor(sf::Color(0, 0, 0, enter));
                }
            }

            void handleInput() const override {
                if (
                    window->getEvent().key.code == sf::Keyboard::Enter ||
                    window->getEvent().key.code == sf::Keyboard::Space
                ) {
                    needsRedraw = true;
                    status = true;
                    time = delayTime;

                    purdah.setFillColor(sf::Color(0, 0, 0, click));
                    handleClick();
                }
            }

            void handleClick() const override {
                if (func) {
                    func();
                }
            }
            
            void handleSetSize() const override {
                purdah.setSize(sf::Vector2f(renderTexture.getSize()));
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-background-button"][0],
                    (unsigned int) json["themes"][theme]["color-background-button"][1],
                    (unsigned int) json["themes"][theme]["color-background-button"][2],
                    (unsigned int) json["themes"][theme]["color-background-button"][3]
                ));
            }
        public:
            Button() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                purdah.setFillColor(sf::Color(0, 0, 0, enter));

                setSize(sf::Vector2u((unsigned int) json["size"]["brick"][0], (unsigned int) json["size"]["brick"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["button"][0], (float) json["position"]["button"][1]));
            }

            void setDelayTime(float newDelayTime) const {
                delayTime = newDelayTime;
            }

            void setOpacityClick(unsigned char newOpacity) const {
                click = newOpacity;
            }

            void setOpacityEnter(unsigned char newOpacity) const {
                enter = newOpacity;
            }

            void setFunctionary(const std::function<void()>& newFunc) const {
                func = newFunc;
            }

            float getDelayTime() const {
                return delayTime;
            }

            unsigned char getOpacityClick() const {
                return click;
            }

            unsigned char getOpacityEnter() const {
                return enter;
            }

            const std::function<void()>& getFunctionary() const {
                return func;
            }
    };
}

#endif
