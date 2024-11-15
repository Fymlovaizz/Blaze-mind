#ifndef __TOOL_OBJECT_LIST_HPP__
#define __TOOL_OBJECT_LIST_HPP__

namespace ng {
    class List : public SampleObject {
        private:
            mutable std::vector<std::string> texts;
            mutable std::vector<sf::Texture> images;
            mutable sf::Sprite imageSprite;

            mutable sf::RectangleShape purdah;

            mutable std::size_t choose = -1;
            mutable unsigned char enter = 50;
            mutable unsigned char click = 100;
            mutable bool status = false;
            mutable float posList = 0.f;

            mutable sf::Text text;
            mutable sf::Font font;
            mutable std::string fontName = "None";

            mutable float opinionHeight = 0.f;
            mutable float listHeight = 0.f;

            mutable std::function<void()> func;

            void checkPtr() const {
                needsRedraw = true;

                if (posList < 0.f) {
                    posList = 0.f;
                }
                else if (posList > listHeight - opinionHeight) {
                    posList = listHeight - opinionHeight;
                }

                handleMove();
            }

            void redraw() const override {
                std::size_t start = posList / opinionHeight;
                std::size_t max = (
                    (start + renderTexture.getSize().y / opinionHeight + 2) < texts.size() ?
                    start + renderTexture.getSize().y / opinionHeight + 2:
                    texts.size()
                );

                for (std::size_t i = start; i != max; ++i) {
                    if (fontName != "None") {
                        text.setString(texts[i]);
                        text.setPosition(opinionHeight * 1.1f, (i + 0.5) * opinionHeight - posList - text.getLocalBounds().height * 0.7f);
                        renderTexture.draw(text);
                    }

                    if (images[i].getSize() != sf::Vector2u()) {
                        imageSprite.setTexture(images[i], true);
                        imageSprite.setScale(
                            images[i].getSize().x > images[i].getSize().y?
                            sf::Vector2f(
                                opinionHeight * 0.8f / images[i].getSize().x,
                                opinionHeight * 0.8f / images[i].getSize().x
                            ):
                            sf::Vector2f(
                                opinionHeight * 0.8f / images[i].getSize().y,
                                opinionHeight * 0.8f / images[i].getSize().y
                            )
                        );
                        imageSprite.setPosition(
                            (opinionHeight - imageSprite.getGlobalBounds().width) / 2.f,
                            (opinionHeight - imageSprite.getGlobalBounds().height) / 2.f + opinionHeight * i - posList
                        );
                        renderTexture.draw(imageSprite);
                    }
                }

                if (choose != -1) {
                    if (start <= choose && choose < max) {
                        purdah.setPosition(0.f, choose * opinionHeight - posList);
                        renderTexture.draw(purdah);
                    }
                }
            }

            void handleEnter() const override {
                float position = (window->getMousePosition().y - sprite.getPosition().y + posList) / opinionHeight;
                if (position < texts.size()) {
                    if (status) {
                        needsRedraw = true;
                        choose = position;
                        status = false;
                        purdah.setFillColor(sf::Color(0, 0, 0, enter));
                    }
                }
            }

            void handleLeave() const override {
                needsRedraw = true;
                choose = -1;
                if (status) {
                    status = false;
                }
            }

            void handlePress() const override {
                float position = (window->getMousePosition().y - sprite.getPosition().y + posList) / opinionHeight;
                if (position < texts.size()) {
                    choose = position;
                    if (!status) {
                        needsRedraw = true;
                        status = true;
                        purdah.setFillColor(sf::Color(0, 0, 0, click));
                    }
                }
            }

            void handleRelease() const override {
                if (status) {
                    needsRedraw = true;
                    status = false;
                    purdah.setFillColor(sf::Color(0, 0, 0, enter));
                }
            }

            void handleClick() const override {
                if (choose != -1) {
                    if (func) {
                        func();
                    }
                }
            }

            void handleExit() const override {
                needsRedraw = true;
                status = false;
                choose = -1;
            }

            void handleMove() const override {
                float position = (window->getMousePosition().y - sprite.getPosition().y + posList) / opinionHeight;
                if (choose != position) {
                    needsRedraw = true;
                    if (position < texts.size()) {
                        choose = position;
                    }
                    else {
                        choose = -1;
                    }
                    if (status) {
                        status = false;
                        purdah.setFillColor(sf::Color(0, 0, 0, enter));
                    }
                }
            }

            void handleDrag() const override {
                posList -= window->getMouseDeltapos().y;
                checkPtr();
            }

            void handleScroll() const override {
                posList -= window->getEvent().mouseWheelScroll.delta * 5.f;
                checkPtr();
            }

            void handleSetSize() const override {
                purdah.setSize({(float) renderTexture.getSize().x, opinionHeight});
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-background-list"][0],
                    (unsigned int) json["themes"][theme]["color-background-list"][1],
                    (unsigned int) json["themes"][theme]["color-background-list"][2],
                    (unsigned int) json["themes"][theme]["color-background-list"][3]
                ));

                setCharacterColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-text-option"][0],
                    (unsigned int) json["themes"][theme]["color-text-option"][1],
                    (unsigned int) json["themes"][theme]["color-text-option"][2],
                    (unsigned int) json["themes"][theme]["color-text-option"][3]
                ));

                setFont(json["themes"][theme]["font-option"]);
            }
        public:
            List() {
                purdah.setFillColor(sf::Color(0, 0, 0, enter));

                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["board"][0], (unsigned int) json["size"]["board"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["list"][0], (float) json["position"]["list"][1]));

                setCharacterSize(json["size_font"][json["other"]["list"]["size"]]);
                setOpinionHeight(json["other"]["list"]["height"]);
            }

            void setFont (const std::string& newFontName) const {
                if (font.loadFromFile(
                    (std::string) json["paths"]["resource"] + 
                    (std::string) json["paths"]["font"] + 
                    newFontName
                )) {
                    if (fontName != newFontName) {
                        needsRedraw = true;
                        fontName = newFontName;
                        text.setFont(font);
                    }
                }
                else {
                    fontName = "None";
                    if (newFontName != "None") {
                        std::cerr << "Warning: Unable to load font file '" << newFontName << "' for 'List'.\n"
                        << "\t- Please check if the file exists and the path is correct.\n"
                        << "\t- The font file must be located in the '"
                        << json["paths"]["resource"] << json["paths"]["font"] << "' folder." << std::endl;
                    }
                }
            }

            void setCharacterColor(const sf::Color& newCharacterColor) const {
                needsRedraw = true;
                text.setFillColor(newCharacterColor);
            }

            void setCharacterSize(unsigned char newCharacterSize) const {
                needsRedraw = true;
                text.setCharacterSize(newCharacterSize);
            }

            void setOpinionHeight(float newOpinionHeight) const {
                needsRedraw = true;
                opinionHeight = newOpinionHeight;
                listHeight = opinionHeight * texts.size();

                purdah.setSize(sf::Vector2f(renderTexture.getSize().x, opinionHeight));
            }

            void setOpacityClick(unsigned char newOpacity) const {
                needsRedraw = true;
                click = newOpacity;
            }

            void setOpacityEnter(unsigned char newOpacity) const {
                needsRedraw = true;
                enter = newOpacity;
            }

            void setFunctionary(const std::function<void()>& newFunc) const {
                func = newFunc;
            }

            const std::string& getFont() const {
                return fontName;
            }

            const sf::Color& getChracterColor() const {
                return text.getFillColor();
            }

            unsigned char getCharacterSize() const {
                return text.getCharacterSize();
            }

            float getOpinionHeight() const {
                return opinionHeight;
            }

            unsigned char getOpacityClick() const {
                return click;
            }

            unsigned char getOpacityEnter() const {
                return enter;
            }

            const std::size_t& getChoose() const {
                return choose;
            }

            const std::function<void()>& getFunctionary() const {
                return func;
            }

            void append(const std::string& text, const std::string& image = "None") const {
                needsRedraw = true;

                texts.emplace_back(text);
                images.emplace_back(sf::Texture());

                if (image != "None") {
                    if (!images.back().loadFromFile(
                        (std::string) json["paths"]["resource"] + 
                        (std::string) json["paths"]["image"] + 
                        image
                    )) {
                        std::cerr << "Warning: Unable to load image file '" << image << "' for 'List'.\n"
                        << "\t- Please check if the file exists and the path is correct.\n"
                        << "\t- The image file must be located in the '"
                        << json["paths"]["resource"] << json["paths"]["image"] << "' folder." << std::endl;
                    }
                }
                else {
                    images.back().setSmooth(true);
                }

                listHeight += opinionHeight;
            }

            void clear() const {
                needsRedraw = true;

                texts.clear();
                images.clear();
                choose = -1;
                if (status) { status = false; }

                listHeight = 0.f;
                posList = 0.f;
            }

    };
}

#endif
