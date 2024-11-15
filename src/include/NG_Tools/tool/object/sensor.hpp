#ifndef __TOOL_OBJECT_SENSOR_HPP__
#define __TOOL_OBJECT_SENSOR_HPP__

namespace ng {
    class Sensor : public SampleObject {
        private:
            mutable sf::RectangleShape purdah;

            mutable float time = 0.f;
            mutable float delayTime = 0.f;
            mutable bool status = false;
            mutable bool press = false;

            unsigned char enter = 40;

            std::function<void()> func;

            void update() const override {
                if (time > 0.f) {
                    time -= window->getDeltatime();
                    if (time <= 0.f) {
                        needsRedraw = true;
                        status = false;
                        if (func) {
                            func();
                        }
                    }
                }
            }

            void redraw() const override {
                if (status) {
                    renderTexture.draw(purdah);
                }
            }

            void handleEnter() const override {
                needsRedraw = true;
                status = true;
                time = 0.f;
                if (func) {
                    func();
                }
            }

            void handleLeave() const override {
                if (!press) {
                    if (delayTime <= 0.f) {
                        status = false;
                        needsRedraw = true;
                        if (func) {
                            func();
                        }
                    }
                    else {
                        time = delayTime;
                    }
                }
            }

            void handleClick() const override {
                press = true;
            }

            void handleExit() const override {
                press = false;
                status = false;
                needsRedraw = true;
                if (func) {
                    func();
                }
            }

            void handleInput() const override {
                if (
                    window->getEvent().key.code == sf::Keyboard::Enter ||
                    window->getEvent().key.code == sf::Keyboard::Space
                ) {
                    if (status && time <= 0.f) {
                        if (func) {
                            func();
                        }
                        return;
                    }
                    if (delayTime > 0.f) {
                        status = true;
                        if (time <= 0.f) {
                            needsRedraw = true;
                        }
                        time = delayTime;
                        if (func) {
                            func();
                        }
                    }
                    else {
                        if (func) {
                            status = true;
                            func();
                            status = false;
                        }
                    }
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
            Sensor() {
                purdah.setFillColor(sf::Color(0, 0, 0, enter));

                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["brick"][0], (unsigned int) json["size"]["brick"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["sensor"][0], (float) json["position"]["sensor"][1]));

                setDelayTime((float) json["other"]["sensor"]["delay"]);
            }

            void setDelayTime(float newDelayTime) const {
                delayTime = newDelayTime;
            }

            void setOpacityEnter(unsigned char newOpacity) {
                enter = newOpacity;
            }

            void setFunctionary(std::function<void()> newFunc) {
                func = newFunc;
            }

            float getDelayTime() const {
                return delayTime;
            }

            unsigned char getOpacityEnter() const {
                return enter;
            }

            std::function<void()> getFunctionary() const {
                return func;
            }

            bool getStatus() const {
                return status;
            }
    };
}

#endif
