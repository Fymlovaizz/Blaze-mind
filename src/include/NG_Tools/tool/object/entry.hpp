#ifndef __TOOL_OBJECT_ENTRY_HPP__
#define __TOOL_OBJECT_ENTRY_HPP__

namespace ng {
    class Entry : public SampleObject {
        private:
            mutable sf::RectangleShape ptr;
            mutable sf::Text text;
            mutable sf::Font font;
            mutable std::string fontName;

            mutable sf::String descripe;
            mutable sf::String input;

            mutable std::size_t cursorPosition;
            mutable float cursorLocation = 0.f;
            mutable float view = 0.f;

            mutable char hideWord = '*';
            mutable bool hideMode = false;

            mutable bool limitNumber = false;
            mutable bool limitAlphabets = false;

            mutable std::size_t limitWordLength = -1;

            mutable float time = 0.5f;
            mutable float delta = 0.5f;
            mutable bool showPtr = true;
            mutable bool inSide = false;

            mutable std::function<void()> funcInput;
            mutable std::function<void()> funcEnter;

            inline void redrawText() const {
                if (!input.isEmpty() || inSide) {
                    if (hideMode) {
                        text.setString(hideWord);
                        cursorLocation = text.getLocalBounds().width * cursorPosition;
                        text.setString(std::wstring(input.getSize(), hideWord));
                    }
                    else {
                        text.setString(input.substring(0, cursorPosition));
                        cursorLocation = text.getLocalBounds().width;
                        text.setString(input);
                    }

                    if (cursorLocation - view > renderTexture.getSize().x - renderTexture.getSize().y) {
                        view = cursorLocation - renderTexture.getSize().x + renderTexture.getSize().y;
                    }
                    else if (cursorLocation - view < 0) {
                        view = cursorLocation;
                    }
                }
                else {
                    text.setString(descripe);
                }

                text.setPosition(renderTexture.getSize().y / 2.f - view, renderTexture.getSize().y * 0.4f);
                renderTexture.draw(text);
            }

            void update() const override {
                if (inSide) {
                    if (time > 0.f) {
                        time -= window->getDeltatime();
                        if (time <= 0.f) {
                            needsRedraw = true;
                            showPtr = !showPtr;
                            time = delta;
                        }
                    }
                }
            }

            void redraw() const override {
                redrawText();

                if (inSide && showPtr) {
                    text.setString(text.getString().substring(0, cursorPosition));
                    ptr.setPosition(
                        text.getPosition().x + text.getLocalBounds().width + 5.f,
                        renderTexture.getSize().y * 0.4f
                    );
                    renderTexture.draw(ptr);
                }
            }

            void handleInput() const override {
                if (!inSide) return;

                needsRedraw = true;
                time = delta;
                showPtr = true;

                if (window->getEvent().key.code == sf::Keyboard::Return) {
                    inSide = false;
                    if (funcEnter) {
                        funcEnter();
                    }
                }

                else if (window->getEvent().key.code == sf::Keyboard::Delete) {
                    if (!input.isEmpty() && cursorPosition < input.getSize()) {
                        input.erase(cursorPosition, 1);
                    }
                }

                else if (window->getEvent().key.code == sf::Keyboard::Left) {
                    if (cursorPosition > 0) {
                        cursorPosition--;
                    }
                }

                else if (window->getEvent().key.code == sf::Keyboard::Right) {
                    if (cursorPosition < input.getSize()) {
                        cursorPosition++;
                    }
                }
            }

            void handleText() const override {
                if (!inSide) return;

                if (window->getEvent().text.unicode == '\b') {
                    if (!input.isEmpty() && cursorPosition > 0) {
                        input.erase(cursorPosition - 1, 1);
                        cursorPosition--;
                    }
                }
                else {
                    if (window->getEvent().text.unicode > 255) return;
                    if (input.getSize() == limitWordLength) return;

                    bool isAlpha = std::isalpha(window->getEvent().text.unicode);
                    bool isDigit = std::isdigit(window->getEvent().text.unicode);

                    if (limitAlphabets && limitNumber) if (!(isAlpha || isDigit)) return;
                    if (limitAlphabets && !isAlpha) return;
                    if (limitNumber && !isDigit) return;

                    input.insert(cursorPosition, static_cast<char>(window->getEvent().text.unicode));
                    cursorPosition++;
                }

                if (funcInput) {
                    funcInput();
                }
            }

            void handlePress() const override {
                needsRedraw = true;
                inSide = true;
            }

            void handleExit() const override {
                needsRedraw = true;
                inSide = false;
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned char) json["themes"][theme]["color-background-label"][0],
                    (unsigned char) json["themes"][theme]["color-background-label"][1],
                    (unsigned char) json["themes"][theme]["color-background-label"][2],
                    (unsigned char) json["themes"][theme]["color-background-label"][3]
                ));

                setPtrColor(sf::Color(
                    (unsigned char) json["themes"][theme]["color-core-main"][0],
                    (unsigned char) json["themes"][theme]["color-core-main"][1],
                    (unsigned char) json["themes"][theme]["color-core-main"][2],
                    (unsigned char) json["themes"][theme]["color-core-main"][3]
                ));

                setPtrOutlineColor(sf::Color(
                    (unsigned char) json["themes"][theme]["color-outline-border-in"][0],
                    (unsigned char) json["themes"][theme]["color-outline-border-in"][1],
                    (unsigned char) json["themes"][theme]["color-outline-border-in"][2],
                    (unsigned char) json["themes"][theme]["color-outline-border-in"][3]
                ));

                setCharacterColor(sf::Color(
                    (unsigned char) json["themes"][theme]["color-text-label"][0],
                    (unsigned char) json["themes"][theme]["color-text-label"][1],
                    (unsigned char) json["themes"][theme]["color-text-label"][2],
                    (unsigned char) json["themes"][theme]["color-text-label"][3]
                ));

                setFont(json["themes"][theme]["font-label"]);

                setPtrThinkness((float) json["themes"][theme]["thinkness-outline-border-in"]);
            }
        public:
            Entry() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["text"][0], (unsigned int) json["size"]["text"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["entry"][0], (float) json["position"]["entry"][1]));

                setPtrSize(sf::Vector2f((float) json["other"]["entry"]["ptr"][0], (float) json["other"]["entry"]["ptr"][1]));
                setPtrDelta((float) json["other"]["entry"]["delta"]);

                setHideMode((bool) json["other"]["entry"]["hide"][0], ((std::string) json["other"]["entry"]["hide"][1])[0]);
                setAlphabetsMode((bool) json["other"]["entry"]["alphabets"]);
                setNumberMode((bool) json["other"]["entry"]["number"]);
                setLimitWordLength((std::size_t) json["other"]["entry"]["limit"]);

                setCharacterSize((unsigned char) json["size_font"][json["other"]["entry"]["size"]]);
                setDescripe(std::translate_to_wstring((std::string) json["other"]["entry"]["descripe"]));
                setText((std::string) json["other"]["entry"]["text"]);
            }

            void setPtrColor(const sf::Color& color) const {
                needsRedraw = true;
                ptr.setFillColor(color);
            }

            void setPtrSize(const sf::Vector2f& size) const {
                needsRedraw = true;
                ptr.setSize(size);
            }

            void setPtrThinkness(float thickness) const {
                needsRedraw = true;
                ptr.setOutlineThickness(thickness);
            }

            void setPtrOutlineColor(const sf::Color& color) const {
                needsRedraw = true;
                ptr.setOutlineColor(color);
            }

            void setPtrDelta(float delta) const {
                time = delta;
                this->delta = delta;
            }

            void setDescripe(const sf::String& descripe) const {
                needsRedraw = true;
                this->descripe = descripe;
            }

            void setText(const sf::String& text) const {
                needsRedraw = true;
                this->input = text;
            }

            void setFont (const std::string& newFontName) const {
                needsRedraw = true;
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

            void setCharacterSize(unsigned char size) const {
                needsRedraw = true;
                text.setCharacterSize(size);
            }

            void setCharacterColor(const sf::Color& color) const {
                needsRedraw = true;
                text.setFillColor(color);
            }

            void setHideMode(bool mode, char word = '*') const {
                if (hideMode != mode || (hideMode && hideWord != word)) {
                    needsRedraw = true;
                    hideMode = mode;
                    hideWord = word;
                }
            }

            void setNumberMode(bool mode) const {
                needsRedraw = true;
                limitNumber = mode;
            }

            void setAlphabetsMode(bool mode) const {
                needsRedraw = true;
                limitAlphabets = mode;
            }

            void setLimitWordLength(const std::size_t& length) const {
                needsRedraw = true;
                limitWordLength = length;
            }

            void setEnterFunctionary(const std::function<void()>& newFunc) const {
                funcEnter = newFunc;
            }

            void setInputFunctionary(const std::function<void()>& newFunc) const {
                funcInput = newFunc;
            }

            const sf::Color& getPtrColor() const {
                return ptr.getFillColor();
            }

            const sf::Vector2f& getPtrSize() const {
                return ptr.getSize();
            }

            float getPtrThinkness() const {
                return ptr.getOutlineThickness();
            }

            const sf::Color& getPtrOutlineColor() const {
                return ptr.getOutlineColor();
            }

            float getPtrDelta() const {
                return delta;
            }

            const sf::String& getDescripe() const {
                return descripe;
            }

            const sf::String& getText() const {
                return input;
            }

            const std::string& getFontName() const {
                return fontName;
            }

            unsigned char getCharacterSize() const {
                return text.getCharacterSize();
            }

            const sf::Color& getCharacterColor() const {
                return text.getFillColor();
            }

            bool getHideMode() const {
                return hideMode;
            }

            char getHideWord() const {
                return hideWord;
            }

            bool getNumberMode() const {
                return limitNumber;
            }

            bool getAlphabetsMode() const {
                return limitAlphabets;
            }

            const std::size_t& getLimitWordLength() const {
                return limitWordLength;
            }

            const std::function<void()>& getEnterFunctionary() const {
                return funcEnter;
            }

            const std::function<void()>& getInputFunctionary() const {
                return funcInput;
            }
    };
}

#endif
