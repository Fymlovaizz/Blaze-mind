#ifndef __TOOL_OBJECT_IMAGE_HPP__
#define __TOOL_OBJECT_IMAGE_HPP__

namespace ng {
    class Image : public SampleObject {
        private:
            mutable sf::Image image;
            mutable std::string imageName;
            mutable sf::Texture texture;
            mutable sf::Sprite textureSprite;

            mutable bool needsReset = false;

            void redraw() const override {
                if (texture.getSize() != sf::Vector2u(0, 0)) {
                    renderTexture.draw(textureSprite);
                }
            }

            void handleSetSize() const override {
                if (texture.getSize() != sf::Vector2u(0, 0) && renderTexture.getSize() != sf::Vector2u(0, 0)) {
                    needsRedraw = true;
                    textureSprite.setScale(sf::Vector2f(renderTexture.getSize()).x / sf::Vector2f(texture.getSize()).x, sf::Vector2f(renderTexture.getSize()).y / sf::Vector2f(texture.getSize()).y);                    
                }
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-background-label"][0],
                    (unsigned int) json["themes"][theme]["color-background-label"][1],
                    (unsigned int) json["themes"][theme]["color-background-label"][2],
                    (unsigned int) json["themes"][theme]["color-background-label"][3]
                ));
            }
        public:
            Image() {
                textureSprite.setPosition(0.f, 0.f);

                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u((unsigned int) json["size"]["ceil"][0], (unsigned int) json["size"]["ceil"][1]));
                setScale({1.f, 1.f});
                setRotation(0.f);
                setPosition(sf::Vector2f((float) json["position"]["image"][0], (float) json["position"]["image"][1]));

                setImage((std::string) json["paths"]["images"]["logo"],true);
            }

            void setImage(const std::string& newImageName, bool size = true) const {
                if (image.loadFromFile(
                    (std::string) json["paths"]["resource"] + 
                    (std::string) json["paths"]["image"] + 
                    newImageName
                )) {
                    imageName = newImageName;
                    texture.loadFromImage(image);
                    textureSprite.setTexture(texture, true);
                    if (size && renderTexture.getSize() != sf::Vector2u(0, 0)) {
                        textureSprite.setScale(sf::Vector2f(renderTexture.getSize()).x / sf::Vector2f(texture.getSize()).x, renderTexture.getSize().y / sf::Vector2f(texture.getSize()).y);
                    }
                    else {
                        renderTexture.create(texture.getSize().x, texture.getSize().y);
                        sprite.setTexture(renderTexture.getTexture(), true);
                    }
                }
                else {
                    std::cerr << "Warning: Unable to load image file '" << newImageName << "' for 'Label'.\n"
                    << "\t- Please check if the file exists and the path is correct.\n"
                    << "\t- The font file must be located in the '"
                    << (std::string) json["paths"]["resource"] << (std::string) json["paths"]["image"]
                    << "' folder." << std::endl;
                }
            }

            std::string getImage() const {
                return imageName;
            }
    };
}

#endif
