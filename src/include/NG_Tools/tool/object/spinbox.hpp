#ifndef __TOOL_OBJECT_SPINBOX_HPP__
#define __TOOL_OBJECT_SPINBOX_HPP__

namespace ng {
    class Spinbox : public SampleObject {
        private:
            mutable sf::RectangleShape up;
            mutable sf::RectangleShape down;
            mutable sf::RectangleShape purdah;

            mutable unsigned char opacity = 90;

            mutable sf::Text text;
            mutable sf::Font font;
            mutable std::string fontName;

            mutable bool which = true;
            mutable float time = 0.f;
            mutable float delayTime = 0.05f;
            mutable bool status = false;
            mutable bool in = false;

            mutable long long max = 1;
            mutable long long min = -1;
            mutable long long value = 0;
            mutable float width;
            mutable unsigned int step = 1;

            mutable bool needsReset = false;

            void update() const override {
                if (time > 0.f) {
                    time -= window->getDeltatime();

                    if (time <= 0.f) {
                        needsRedraw = true;
                        status = false;
                        time = 0.f;
                    }
                }
            }

            void redraw() const override {
                if (needsReset) {
                    needsReset = false;

                    text.setString(std::to_string(value));
                    text.setPosition(
                        (renderTexture.getSize().x - width - text.getGlobalBounds().width) / 2.f, 
                        (renderTexture.getSize().y - text.getGlobalBounds().height) / 2.f
                    );
                }

                if (value != max) {
                    renderTexture.draw(up);
                }

                if (value != min) {
                    renderTexture.draw(down);
                }

                if (status) {
                    if (which) {
                        purdah.setPosition(up.getPosition());
                    }
                    else {
                        purdah.setPosition(down.getPosition());
                    }
                    renderTexture.draw(purdah);
                }

                renderTexture.draw(text);
            }

            void handlePress() const override {
                if (
                    (window->getMousePosition().x - sprite.getPosition().x - sprite.getOrigin().x) / sprite.getScale().x
                    > renderTexture.getSize().x - width
                ) {
                    needsRedraw = true;
                    status = true;
                    in = true;
                    (
                        (window->getMousePosition().y - sprite.getPosition().y - sprite.getOrigin().y) / sprite.getScale().y 
                        < renderTexture.getSize().y / 2.f
                    )? which = true: which = false;
                }
            }

            void handleRelease() const override {
                needsRedraw = true;
                status = false;
            }

            void handleClick() const override {
                if (in) which? upValue(): downValue();
                in = false;
            }

            void handleInput() const override {
                if (
                    window->getEvent().key.code == sf::Keyboard::Right ||
                    window->getEvent().key.code == sf::Keyboard::Up
                ) {
                    needsRedraw = true;
                    status = true;
                    time = delayTime;
                    upValue();
                    return;
                }

                if (
                    window->getEvent().key.code == sf::Keyboard::Left ||
                    window->getEvent().key.code == sf::Keyboard::Down
                ) {
                    needsRedraw = true;
                    status = true;
                    time = delayTime;
                    downValue();
                    return;
                }

                if (
                    window->getEvent().key.code == sf::Keyboard::Return ||
                    window->getEvent().key.code == sf::Keyboard::Space
                ) {
                    needsRedraw = true;
                    status = true;
                    time = delayTime;
                    which? upValue(): downValue();
                    return;
                }
            }

            void handleSetSize() const override {
                purdah.setSize({width, renderTexture.getSize().y / 2.f});
                up.setSize({width, renderTexture.getSize().y / 2.f});
                down.setSize({width, renderTexture.getSize().y / 2.f});

                up.setPosition({renderTexture.getSize().x - width, 0.f});
                down.setPosition({renderTexture.getSize().x - width, renderTexture.getSize().y / 2.f});                
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-background-button"][0],
                    (unsigned int) json["themes"][theme]["color-background-button"][1],
                    (unsigned int) json["themes"][theme]["color-background-button"][2],
                    (unsigned int) json["themes"][theme]["color-background-button"][3]
                ));

                setCharacterColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-text-option"][0],
                    (unsigned int) json["themes"][theme]["color-text-option"][1],
                    (unsigned int) json["themes"][theme]["color-text-option"][2],
                    (unsigned int) json["themes"][theme]["color-text-option"][3]
                ));

                setUpColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-main"][0],
                    (unsigned int) json["themes"][theme]["color-core-main"][1],
                    (unsigned int) json["themes"][theme]["color-core-main"][2],
                    (unsigned int) json["themes"][theme]["color-core-main"][3]
                ));

                setDownColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-secondary"][0],
                    (unsigned int) json["themes"][theme]["color-core-secondary"][1],
                    (unsigned int) json["themes"][theme]["color-core-secondary"][2],
                    (unsigned int) json["themes"][theme]["color-core-secondary"][3]
                ));

                setFont(json["themes"][theme]["font-option"]);
            }
        public:
            Spinbox() {
                purdah.setFillColor(sf::Color(0, 0, 0, opacity));

                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["brick"][0], (unsigned int) json["size"]["brick"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["spinbox"][0], (float) json["position"]["spinbox"][1]));

                setCharacterSize((unsigned char) json["size_font"][json["other"]["spinbox"]["size"]]);

                setWidth((float) json["other"]["spinbox"]["width"]);

                setMax((long long) json["other"]["spinbox"]["max"]);
                setMin((long long) json["other"]["spinbox"]["min"]);
                setStep((unsigned int) json["other"]["spinbox"]["step"]);
                setValue((long long) json["other"]["spinbox"]["value"]);

                setDelayTime((float) json["other"]["spinbox"]["delay"]);
                setOpacity((unsigned char) json["other"]["spinbox"]["opacity"]);
            }

            void setFont (const std::string& newFontName) const {
                needsRedraw = true;
                needsReset = true;
                if (font.loadFromFile(
                    (std::string) json["paths"]["resource"] + 
                    (std::string) json["paths"]["font"] + 
                    newFontName
                )) {
                    fontName = newFontName;
                    text.setFont(font);
                }
                else {
                    std::cerr << "Warning: Unable to load font file '" << newFontName << "' for 'Label'.\n"
                    << "\t- Please check if the file exists and the path is correct.\n"
                    << "\t- The font file must be located in the '"
                    << json["paths"]["resource"] << json["paths"]["font"]  << "' folder." << std::endl;
                }
            }

            void setCharacterColor(const sf::Color& newColor) const {
                needsRedraw = true;
                text.setFillColor(newColor);
            }

            void setCharacterSize(unsigned char newCharacterSize) const {
                needsRedraw = true;
                needsReset = true;
                text.setCharacterSize(newCharacterSize);
            }

            void setUpColor(const sf::Color& newColor) const {
                up.setFillColor(newColor);
            }

            void setDownColor(const sf::Color& newColor) const {
                down.setFillColor(newColor);
            }

            void setWidth(float newWidth) const {
                needsRedraw = true;
                needsReset = true;
                width = newWidth;
                handleSetSize();
            }

            void setMax(const long long& max) const {
                if (max < min) return;
                if (value > max) {
                    needsReset = true;
                    value = max;
                }
                if (value == this->max || value == max) needsRedraw = true;
                this->max = max;
            }

            void setMin(const long long& min) const {
                if (max < min) return;
                if (value < min) {
                    needsReset = true;
                    value = min;
                }
                if (value == this->min || value == min) needsRedraw = true;
                this->min = min;
            }

            void setStep(unsigned int newStep) const {
                step = newStep;
            }

            void setValue(const long long& value) const {
                if (value > max) return;
                if (value < min) return;

                needsRedraw = true;
                needsReset = true;
                this->value = value;
            }

            void setDelayTime(float newDelayTime) const {
                delayTime = newDelayTime;
            }

            void setOpacity(unsigned char newOpacity) const {
                purdah.setFillColor(sf::Color(0, 0, 0, newOpacity));
                opacity = newOpacity;
            }

            const std::string& getFont() const {
                return fontName;
            }

            const sf::Color& getCharacterColor() const {
                return text.getFillColor();
            }

            unsigned char getCharacterSize() const {
                return text.getCharacterSize();
            }

            const sf::Color& getUpColor() const {
                return up.getFillColor();
            }

            const sf::Color& getDownColor() const {
                return down.getFillColor();
            }

            float getWidth() const {
                return width;
            }

            const long long& getMax() const {
                return max;
            }

            const long long& getMin() const {
                return min;
            }

            unsigned int getStep() const {
                return step;
            }

            const long long& getValue() const {
                return value;
            }

            float getDelayTime() const {
                return delayTime;
            }

            unsigned char getOpacity() const {
                return opacity;
            }

            inline void upValue() const {
                which = true;
                setValue(value + step);
            }

            inline void downValue() const {
                which = false;
                setValue(value - step);
            }
    };
}

#endif
