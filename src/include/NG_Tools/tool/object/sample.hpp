#ifndef __TOOL_OBJECT_SAMPLE_HPP__
#define __TOOL_OBJECT_SAMPLE_HPP__

namespace ng {
    class Window;
    class Frame;

    class SampleObject {
        protected:
            friend Window;
            friend Frame;

            mutable Window* window = nullptr;
            mutable SampleObject* parent = nullptr;
            nlohmann::json json;

            mutable sf::Color color = sf::Color::Transparent;

            mutable bool isVisible = true;
            mutable bool canInteract = true;
            mutable bool needsRedraw = false;

            mutable sf::RenderTexture renderTexture;
            mutable sf::Sprite sprite;

            virtual void update() const {}
            virtual void redraw() const {}
            void draw(sf::RenderTarget& target) const {
                if (needsRedraw) {
                    needsRedraw = false;
                    renderTexture.clear(color);
                    redraw();
                    renderTexture.display();
                }
                if (isVisible) {
                    target.draw(sprite);
                }
            }

            virtual void handleEnter() const {}
            virtual void handleLeave() const {}

            virtual void handleAlt() const {}
            virtual void handleCtrl() const {}
            virtual void handleShift() const {}
            virtual void handleInput() const {}
            virtual void handleText() const {}

            virtual void handlePress() const {}
            virtual void handleHold() const {}
            virtual void handleRelease() const {}
            virtual void handleClick() const {}
            virtual void handleDrop() const {}
            virtual void handleExit() const {}

            virtual void handleMove() const {}
            virtual void handleScroll() const {}
            virtual void handleDrag() const {}

            virtual void handleAppend() const {}
            virtual void handleShowMode() const {}
            virtual void handleInteractMode() const {}

            virtual void handleSetColor() const {}
            virtual void handleSetSize() const {}
            virtual void handleSetScale() const {}
            virtual void handleSetPosition() const {}
            virtual void handleSetOrigin() const {}
            virtual void handleSetRotation() const {}

            virtual void handleSetTheme(int theme) const {}

            void setStorage(SampleObject* object) const {
                parent = object;
            }

        public:
            bool isWithinObject(const sf::Vector2f& pos) const {
                if (pos.x < sprite.getPosition().x + sprite.getOrigin().x) {
                    return false;
                }
                if (pos.x > sprite.getPosition().x + sprite.getOrigin().x + renderTexture.getSize().x * sprite.getScale().x) {
                    return false;
                }
                if (pos.y < sprite.getPosition().y + sprite.getOrigin().y) {
                    return false;
                }
                if (pos.y > sprite.getPosition().y + sprite.getOrigin().y + renderTexture.getSize().y * sprite.getScale().y) {
                    return false;
                }
                return canInteract;
            }

            void setColor(const sf::Color& newColor) const { 
                needsRedraw = true;
                color = newColor;
                handleSetColor();
            }

            void setSize(const sf::Vector2u& newSize) const { 
                needsRedraw = true;
                renderTexture.create(newSize.x, newSize.y);
                sprite.setTexture(renderTexture.getTexture(), true);
                handleSetSize();
            }

            void setScale(const sf::Vector2f& newScale) const { 
                needsRedraw = true;
                sprite.setScale(newScale);
                handleSetScale();
            }

            void setPosition(const sf::Vector2f& newPosition) const { 
                needsRedraw = true;
                sprite.setPosition(newPosition);
                handleSetPosition();
            }

            void setOrigin(const sf::Vector2f& newOrigin) const {
                needsRedraw = true;
                sprite.setOrigin(newOrigin);
                handleSetOrigin();
            }

            void setRotation(float newAngle) const {
                needsRedraw = true;
                sprite.setRotation(newAngle);
                handleSetRotation();
            }

            const sf::Color& getColor() const { 
                return color;
            }

            const sf::Vector2u getSize() const { 
                return renderTexture.getSize();
            }

            const sf::Vector2f& getScale() const { 
                return sprite.getScale();
            }

            const sf::Vector2f& getPosition() const { 
                return sprite.getPosition();
            }

            const sf::Vector2f& getOrigin() const {
                return sprite.getOrigin();
            }

            float getRotation() const { 
                return sprite.getRotation();
            }

            void setShowMode(bool newMode) const { 
                needsRedraw = true;
                isVisible = newMode;
                handleShowMode();
            }

            bool getShowMode() const { 
                return isVisible;
            }

            void setInteractMode(bool newMode) const { 
                canInteract = newMode;
                handleInteractMode();
            }

            bool getInteractMode() const { 
                return canInteract;
            }

            bool getRedraw() const {
                return needsRedraw;
            }

            virtual ~SampleObject() = default;
    };
}

#endif
