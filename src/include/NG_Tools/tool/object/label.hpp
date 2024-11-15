#ifndef __TOOL_OBJECT_LABEL_HPP__
#define __TOOL_OBJECT_LABEL_HPP__

namespace ng {
    class Label : public SampleObject {
        private:
            mutable sf::String describe;
            mutable sf::Text text;

            mutable std::string fontName;
            mutable sf::Font font;

            mutable bool needsReset = false;

            mutable unsigned char type = 0;
            mutable float view = 0.f;
            mutable float speed = 50.f;

            void update() const override {
                if (type == 7 || type == 8) {
                    view -= window->getDeltatime() * speed;
                    if (view < -text.getLocalBounds().width) {
                        view = renderTexture.getSize().x * 1.f;
                    }
                    needsRedraw = true;
                    needsReset = true;
                }
            }

            void redraw() const override {
                if (needsReset) {
                    switch (type) {
                        case 1:
                            text.setPosition(0.f, (renderTexture.getSize().y - text.getGlobalBounds().height) / 2.f);
                            break;
                        case 2:
                            text.setPosition((renderTexture.getSize().x - text.getGlobalBounds().width) / 2.f, 0.f);
                            break;
                        case 3:
                            text.setPosition(
                                (renderTexture.getSize().x - text.getGlobalBounds().width) / 2.f, 
                                (renderTexture.getSize().y - text.getGlobalBounds().height) / 2.f
                            );
                            break;
                        case 4:
                            text.setPosition(
                                renderTexture.getSize().x - text.getGlobalBounds().width, 
                                renderTexture.getSize().y - text.getGlobalBounds().height
                            );
                            break;
                        case 5:
                            text.setPosition(0.f, renderTexture.getSize().y / 2.f - text.getGlobalBounds().height / 1.25f);
                            break;
                        case 6:
                            text.setPosition(
                                (renderTexture.getSize().x - text.getGlobalBounds().width) / 2.f, 
                                renderTexture.getSize().y / 2.f - text.getGlobalBounds().height / 1.25f
                            );
                            break;
                        case 7:
                            text.setPosition(
                                view,
                                (renderTexture.getSize().y - text.getGlobalBounds().height) / 2.f
                            );
                            break;
                        case 8:
                            text.setPosition(
                                view,
                                renderTexture.getSize().y / 2.f - text.getGlobalBounds().height / 1.25f
                            );
                            break;
                        default:
                            text.setPosition(0.f, 0.f);
                            break;
                    }
                }
                renderTexture.draw(text);
            }

            void handleSetSize() const override {
                needsReset = true;
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-background-label"][0],
                    (unsigned int) json["themes"][theme]["color-background-label"][1],
                    (unsigned int) json["themes"][theme]["color-background-label"][2],
                    (unsigned int) json["themes"][theme]["color-background-label"][3]
                ));

                setCharacterColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-text-label"][0],
                    (unsigned int) json["themes"][theme]["color-text-label"][1],
                    (unsigned int) json["themes"][theme]["color-text-label"][2],
                    (unsigned int) json["themes"][theme]["color-text-label"][3]
                ));

                setFont(json["themes"][theme]["font-label"]);
            }
        public:
            Label() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setDescribe(std::translate_to_wstring(json["other"]["label"]["text"]));
                setCharacterSize((unsigned char) json["size_font"][json["other"]["label"]["size"]]);

                setSize(sf::Vector2u((unsigned int) json["size"]["text"][0],(unsigned int) json["size"]["text"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["label"][0], (float) json["position"]["label"][1]));

                setType((unsigned char) json["other"]["label"]["type"]);
                setSpeed((float) json["other"]["label"]["speed"]);
            }

            void setDescribe(const sf::String& newDescribe) const {
                needsRedraw = true;
                needsReset = true;
                describe = newDescribe;
                text.setString(newDescribe);
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

            void setType(unsigned char newType) const {
                needsRedraw = true;
                needsReset = true;
                type = newType;
                if (type == 7 || type == 8) {
                    view = renderTexture.getSize().x * 1.f;
                }
            }

            void setSpeed(float newSpeed) const {
                speed = newSpeed;
            }

            const sf::String& getDescribe() const {
                return describe;
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

            unsigned char getType() const {
                return type;
            }

            float getSpeed() const {
                return speed;
            }
    };
}

#endif
