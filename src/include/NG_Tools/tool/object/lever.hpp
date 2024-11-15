#ifndef __TOOL_OBJECT_LEVER_HPP__
#define __TOOL_OBJECT_LEVER_HPP__

namespace ng {
    class Lever : public SampleObject {
        private:
            mutable sf::RectangleShape purdah;

            mutable float time = 0.f;
            mutable float delayTime = 0.05f;
            mutable bool status = false;
            mutable bool inclick = false;
            mutable bool inside = false;

            mutable unsigned char enter = 30;
            mutable unsigned char click = 90;
            mutable unsigned char press = 70;

            mutable std::function<void()> func;

            void update() const override {
                if (time > 0.f) {
                    time -= window->getDeltatime();
                    if (time <= 0.f) {
                        needsRedraw = true;
                        inclick = false;

                        purdah.setFillColor(sf::Color(0, 0, 0, status ? press : enter));
                    }
                }
            }

            void redraw() const override {
                if (inside || inclick || status) {
                    renderTexture.draw(purdah);
                }
            }

            void handleEnter() const override {
                needsRedraw = true;
                inside = true;
            }

            void handleLeave() const override {
                inside = false;
                if (time <= 0.f) {
                    needsRedraw = true;
                }
            }

            void handlePress() const override {
                if (!inclick) {
                    needsRedraw = true;
                    inclick = true;

                    purdah.setFillColor(sf::Color(0, 0, 0, click));
                }
            }

            void handleRelease() const override {
                if (inclick) {
                    needsRedraw = true;
                    inclick = false;

                    purdah.setFillColor(sf::Color(0, 0, 0, status ? press : enter));
                }
            }

            void handleInput() const override {
                if (
                    window->getEvent().key.code == sf::Keyboard::Enter ||
                    window->getEvent().key.code == sf::Keyboard::Space
                ) {
                    needsRedraw = true;
                    inclick = true;
                    status = !status;
                    time = delayTime;

                    purdah.setFillColor(sf::Color(0, 0, 0, click));
                    if (func) {
                        func();
                    }
                }
            }

            void handleClick() const override {
                status = !status;

                purdah.setFillColor(sf::Color(0, 0, 0, status ? press : enter));
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
            Lever() {
                purdah.setFillColor(sf::Color(0, 0, 0, enter));

                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["brick"][0], (unsigned int) json["size"]["brick"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["lever"][0], (float) json["position"]["lever"][1]));
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

            void setOpacityPress(unsigned char newOpacity) const {
                press = newOpacity;
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

            unsigned char getOpacityPress() const {
                return press;
            }

            const std::function<void()>& getFunctionary() const {
                return func;
            }

            bool getStatus() const {
                return status;
            }
    };
}

#endif
