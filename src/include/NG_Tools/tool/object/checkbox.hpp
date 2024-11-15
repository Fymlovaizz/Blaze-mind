#ifndef __TOOL_OBJECT_CHECKBOX_HPP__
#define __TOOL_OBJECT_CHECKBOX_HPP__

namespace ng {
    class CheckBox : public SampleObject {
        private:
            mutable float headRadius;
            mutable float brdrRadius;
            mutable sf::RectangleShape non;
            mutable sf::RectangleShape headRectangle;
            mutable sf::RectangleShape brdrRectangle;
            mutable sf::ConvexShape triangle;

            mutable std::size_t ptr = -1;

            mutable sf::Text text;
            mutable sf::Font font;
            mutable std::string fontName;
            mutable std::vector<std::string> texts;

            mutable float opinionHeight = 0.f;
            mutable float listHeight = 0.f;
            mutable float posList = 0.f;

            mutable std::vector<bool> status;
            mutable std::vector<bool> canInteracts;
            mutable std::function<void()> func;

            void check() const {
                needsRedraw = true;

                if (posList < 0.f) {
                    posList = 0.f;
                }
                else if (posList > listHeight - opinionHeight) {
                    posList = listHeight - opinionHeight;
                }
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
                        text.setPosition(
                            opinionHeight * 2.1f, 
                            (i + 0.5) * opinionHeight - posList - text.getLocalBounds().height * 0.7f
                        );
                        renderTexture.draw(text);
                    }

                    if (!canInteracts[i]) {
                        non.setPosition(0.f, i * opinionHeight - posList);
                        renderTexture.draw(non);
                    }

                    brdrRectangle.setPosition(opinionHeight * 1.5f, i * opinionHeight - posList + opinionHeight / 2.f);
                    renderTexture.draw(brdrRectangle);

                    if (status[i]) {
                        headRectangle.setPosition(opinionHeight * 1.5f, i * opinionHeight - posList + opinionHeight / 2.f);
                        renderTexture.draw(headRectangle);
                    }
                }

                if (ptr != -1) {
                    if (start <= ptr && ptr < max) {
                        triangle.setPosition(0.f, ptr * opinionHeight - posList);
                        renderTexture.draw(triangle);
                    }
                }
            }

            void handleInput() const override {
                if (ptr == -1) {
                    return;
                }

                if (
                    window->getEvent().key.code == sf::Keyboard::Space ||
                    window->getEvent().key.code == sf::Keyboard::Enter
                ) {
                    if (canInteracts[ptr]) {
                        needsRedraw = true;
                        status[ptr] = !status[ptr];
                    }
                }

                if (
                    window->getEvent().key.code == sf::Keyboard::Up ||
                    window->getEvent().key.code == sf::Keyboard::Left
                ) {
                    if (ptr > 0) {
                        needsRedraw = true;
                        ptr -= 1;
                    }
                }

                if (
                    window->getEvent().key.code == sf::Keyboard::Down ||
                    window->getEvent().key.code == sf::Keyboard::Right
                ) {
                    if (ptr < texts.size() - 1) {
                        needsRedraw = true;
                        ptr += 1;
                    }
                }
            }

            void handleClick() const override {
                ptr = (window->getMousePosition().y - sprite.getPosition().y + posList) / opinionHeight;
                if (canInteracts[ptr]) {
                    needsRedraw = true;
                    status[ptr] = !status[ptr];
                }
            }

            void handleExit() const override {
                needsRedraw = true;
                ptr = -1;
            }

            void handleDrag() const override {
                posList -= window->getMouseDeltapos().y;
                check();
            }

            void handleScroll() const override {
                posList -= window->getEvent().mouseWheelScroll.delta * 5.f;
                check();
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

                setBoxColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-main"][0],
                    (unsigned int) json["themes"][theme]["color-core-main"][1],
                    (unsigned int) json["themes"][theme]["color-core-main"][2],
                    (unsigned int) json["themes"][theme]["color-core-main"][3]
                ));

                setColorNonInteract(sf::Color(
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][0],
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][1],
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][2],
                    (unsigned int) json["themes"][theme]["color-outline-border-out"][3]
                ));

                setPtrColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-core-main"][0],
                    (unsigned int) json["themes"][theme]["color-core-main"][1],
                    (unsigned int) json["themes"][theme]["color-core-main"][2],
                    (unsigned int) json["themes"][theme]["color-core-main"][3]
                ));

                setPtrOutlineColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][0],
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][1],
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][2],
                    (unsigned int) json["themes"][theme]["color-outline-border-in"][3]
                ));

                setFont(json["themes"][theme]["font-option"]);

                setBrdrThinkness(json["themes"][theme]["thinkness-outline-choose"]);
                setPtrThinkness(json["themes"][theme]["thinkness-outline-border-in"]);
            }
        public:
            CheckBox() {
                brdrRectangle.setFillColor(sf::Color(0, 0, 0, 0));
                triangle.setPointCount(3);

                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["board"][0],(unsigned int) json["size"]["board"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["checkbox"][0], (float) json["position"]["checkbox"][1]));

                setHeadRadius(json["other"]["checkbox"]["radius"][0]);
                setBrdrRadius(json["other"]["checkbox"]["radius"][1]);

                setCharacterSize(json["size_font"][json["other"]["checkbox"]["size"]]);
                setOpinionHeight(json["other"]["checkbox"]["height"]);
            }

            void setBoxColor(const sf::Color& newColor) const {
                needsRedraw = true;
                headRectangle.setFillColor(newColor);
                brdrRectangle.setOutlineColor(newColor);
            }

            void setColorNonInteract(const sf::Color& newColor) const {
                needsRedraw = true;
                non.setFillColor(newColor);                
            }

            void setHeadRadius(float radius) const {
                needsRedraw = true;
                headRadius = radius;
                headRectangle.setSize({radius * 2.f, radius * 2.f});
                headRectangle.setOrigin(radius, radius);
            }

            void setBrdrRadius(float radius) const {
                needsRedraw = true;
                brdrRadius = radius;
                brdrRectangle.setSize({radius * 2.f, radius * 2.f});
                brdrRectangle.setOrigin(radius, radius);
            }

            void setBrdrThinkness(float thickness) const {
                needsRedraw = true;
                brdrRectangle.setOutlineThickness(thickness);
            }

            void setPtrColor(const sf::Color& newColor) const {
                needsRedraw = true;
                triangle.setFillColor(newColor);
            }

            void setPtrThinkness(float thickness) const {
                needsRedraw = true;
                triangle.setOutlineThickness(thickness);
            }

            void setPtrOutlineColor(const sf::Color& newColor) const {
                needsRedraw = true;
                triangle.setOutlineColor(newColor);
            }

            void setOpinionHeight(float height) const {
                needsRedraw = true;
                opinionHeight = height;
                listHeight = height * texts.size();
                triangle.setPoint(0, sf::Vector2f(height * 0.3f, height * 0.3f));
                triangle.setPoint(1, sf::Vector2f(height * 0.7f, height * 0.5f));
                triangle.setPoint(2, sf::Vector2f(height * 0.3f, height * 0.7f));
                non.setSize({height * 0.2f, height});
            }

            void setText(const std::size_t& index, const std::string& newText) const {
                if (index >= texts.size()) {
                    std::cerr << "Error: Index out of range for 'CheckBox'." << std::endl;
                }
                needsRedraw = true;
                texts[index] = newText;
            }

            void setFont(const std::string& newFontName) const {
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
                        std::cerr << "Warning: Unable to load font file '" << newFontName << "' for 'CheckBox'.\n"
                        << "\t- Please check if the file exists and the path is correct.\n"
                        << "\t- The font file must be located in the '"
                        << json["paths"]["resource"] << json["paths"]["font"] << "' folder." << std::endl;
                    }
                }
            }

            void setCharacterColor(const sf::Color& newColor) const {
                needsRedraw = true;
                text.setFillColor(newColor);
            }

            void setCharacterSize(unsigned char size) const {
                needsRedraw = true;
                text.setCharacterSize(size);
            }

            void setCanInteract(const std::size_t& index, bool canInteract) const {
                if (index >= canInteracts.size()) {
                    std::cerr << "Error: Index out of range for 'CheckBox'." << std::endl;
                    return;
                }

                needsRedraw = true;
                canInteracts[index] = canInteract;
            }

            void setStatus(const std::size_t& index, bool status) const { 
                if (index >= this->status.size()) {
                    std::cerr << "Error: Index out of range for 'CheckBox'." << std::endl;
                    return;
                }

                needsRedraw = true;
                this->status[index] = status;
            }

            void setFunctionary(const std::function<void()>& func) const {
                this->func = func;
            }

            const sf::Color& getBoxColor() const {
                return headRectangle.getFillColor();
            }

            const sf::Color& getNonInteractColor() const {
                return non.getFillColor();
            }

            float getHeadRadius() const {
                return headRadius;
            }

            float getBrdrRadius() const {
                return brdrRadius;
            }

            float getBrdrThinkness() const {
                return brdrRectangle.getOutlineThickness();
            }

            const sf::Color& getPtrColor() const {
                return triangle.getFillColor();
            }

            float getPtrThinkness() const {
                return triangle.getOutlineThickness();
            }

            const sf::Color& getPtrOutlineColor() const {
                return triangle.getOutlineColor();
            }

            float getOpinionHeight() const {
                return opinionHeight;
            }

            const std::string& getText(const std::size_t& index) const {
                if (index >= texts.size()) {
                    std::cerr << "Error: Index out of range for 'CheckBox'." << std::endl;
                    #pragma GCC diagnostic push
                    #pragma GCC diagnostic ignored "-Wreturn-local-addr"
                    return "[None]";
                    #pragma GCC diagnostic pop
                }
                return texts[index];
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

            bool getCanInteract(const std::size_t& index) const {
                if (index >= canInteracts.size()) {
                    std::cerr << "Error: Index out of range for 'CheckBox'." << std::endl;
                    return false;
                }
                return canInteracts[index];
            }

            bool getStatus(const std::size_t& index) const {
                if (index >= status.size()) {
                    std::cerr << "Error: Index out of range for 'CheckBox'." << std::endl;
                    return false;
                }
                return status[index];
            }

            const std::function<void()>& getFunctionary() const {
                return func;
            }

            const std::vector<std::string>& getTexts() const {
                return texts;
            }

            const std::vector<bool>& getCanInteracts() const {
                return canInteracts;
            }

            const std::vector<bool>& getStatus() const {
                return status;
            }

            void append(const std::string& text, bool status = false, bool canInteract = true) const {
                needsRedraw = true;

                this->texts.push_back(text);
                this->status.push_back(status);
                this->canInteracts.push_back(canInteract);

                listHeight += opinionHeight;
            }

            void clear() const {
                needsRedraw = true;

                texts.clear();
                canInteracts.clear();
                status.clear();
                ptr = -1;
                posList = 0.f;
                listHeight = 0.f;
            }
    };
}

#endif
