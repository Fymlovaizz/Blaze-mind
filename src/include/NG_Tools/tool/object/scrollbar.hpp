#ifndef __TOOL_OBJECT_SCROLLBAR_HPP__
#define __TOOL_OBJECT_SCROLLBAR_HPP__

namespace ng {
    class Scrollbar : public SampleObject {
        private:
            bool type = true;

            mutable float volume = 0.f;
            mutable float view = 0.f;
            mutable sf::RectangleShape head;

            mutable bool needsResetView = false;
            mutable bool needsResetVolume = false;

            std::function<void()> func;

            void redraw() const override {
                if (renderTexture.getSize() == sf::Vector2u()) {
                    return;
                }

                if (needsResetVolume) {
                    needsResetVolume = false;

                    if (type) {
                        head.setSize(sf::Vector2f(volume, renderTexture.getSize().y));
                    }
                    else {
                        head.setSize(sf::Vector2f(renderTexture.getSize().x, volume));
                    }
                }

                if (needsResetView) {
                    needsResetView = false;

                    if (view < 0.f) {
                        view = 0.f;
                    }

                    if (type) {
                        if (view + volume > renderTexture.getSize().x) {
                            view = renderTexture.getSize().x - volume;
                        }

                        head.setPosition(view, 0.f);
                    }                       
                    else {
                        if (view + volume > renderTexture.getSize().y) {
                            view = renderTexture.getSize().y - volume;
                        }
                        head.setPosition(0.f, view);
                    }                 
                }

                renderTexture.draw(head);
            }

            void handlePress() const override {
                setView(
                    type?   (window->getMousePosition().x - sprite.getPosition().x - sprite.getOrigin().x)
                            / sprite.getScale().x - head.getSize().x / 2
                        :   (window->getMousePosition().y - sprite.getPosition().y - sprite.getOrigin().y)
                            / sprite.getScale().x - head.getSize().y / 2
                );
            }

            void handleDrag() const override {
                setView(
                    view + (
                        type? (window->getMouseDeltapos().x * sprite.getScale().x) 
                            : (window->getMouseDeltapos().y * sprite.getScale().y)
                    )
                );
            }

            void handleScroll() const override {
                setView(view + window->getEvent().mouseWheelScroll.delta * 5.f);
            }

            void handleSetSize() const override {
                if (type) {
                    if (volume > renderTexture.getSize().x) {
                        volume = renderTexture.getSize().x;
                    }
                }
                else {
                    if (volume > renderTexture.getSize().x) {
                        volume = renderTexture.getSize().x;
                    }
                }

                needsRedraw = true;
                needsResetView = true;
                needsResetVolume = true;
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-semi-bar"][0],
                    (unsigned int) json["themes"][theme]["color-semi-bar"][1],
                    (unsigned int) json["themes"][theme]["color-semi-bar"][2],
                    (unsigned int) json["themes"][theme]["color-semi-bar"][3]
                ));

                setHeadColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-main"][0],
                    (unsigned int) json["themes"][theme]["color-core-main"][1],
                    (unsigned int) json["themes"][theme]["color-core-main"][2],
                    (unsigned int) json["themes"][theme]["color-core-main"][3]
                ));
            }
        public:
            Scrollbar() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["slider"][0], (unsigned int) json["size"]["slider"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["scrollbar"][0], (float) json["position"]["scrollbar"][1]));

                setVolume((float) json["other"]["scrollbar"]["volume"]);
                setView((float) json["other"]["scrollbar"]["view"]);
                setType((bool) json["other"]["scrollbar"]["type"]);
            }

            void setType(bool newType) {
                if (type != newType) {
                    type = newType;

                    needsRedraw = true;
                    needsResetView = true;
                    needsResetVolume = true;

                    volume = volume / (type? renderTexture.getSize().y : renderTexture.getSize().x) 
                                    * (type? renderTexture.getSize().x : renderTexture.getSize().y);
                    view = view / (type? renderTexture.getSize().y : renderTexture.getSize().x)
                                * (type? renderTexture.getSize().x : renderTexture.getSize().y);
                }
            }

            void setHeadColor(sf::Color newHeadColor) const {
                needsRedraw = true;
                head.setFillColor(newHeadColor);
            }

            void setVolume(float newVolume) {
                if (newVolume > (type? renderTexture.getSize().x : renderTexture.getSize().y) || newVolume < 0.f) {
                    std::cout << newVolume << std::endl;
                    std::cerr << "Error: Volume must be between 0 and " << 
                    std::to_string(type? renderTexture.getSize().x : renderTexture.getSize().y) << 
                    '!' << std::endl;
                    return;
                }
                volume = newVolume;

                needsRedraw = true;
                needsResetView = true;
                needsResetVolume = true;
            }

            void setView(float newView) const {
                view = newView;

                needsRedraw = true;
                needsResetView = true;

                if (func) {
                    func();
                }
            }

            void setFunctionary(std::function<void()> newFunc) {
                func = newFunc;
            }

            bool getType() const {
                return type;
            }

            sf::Color getHeadColor() const {
                return head.getFillColor();
            }

            float getVolume() const {
                return volume;
            }

            float getView() const {
                return view;
            }

            std::function<void()> getFunctionary() const {
                return func;
            }
    };
}

#endif
