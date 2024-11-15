#ifndef __TOOL_OBJECT_FRAME_HPP__
#define __TOOL_OBJECT_FRAME_HPP__

namespace ng {
    class Frame : public SampleObject {
        private:
            mutable std::vector<SampleObject*> frame = {};

            void redraw() const override {
                if (!frame.empty()) {
                    for (auto& object : frame) {
                        object->draw(renderTexture);
                    }
                }
            }

            void handleSetTheme(int theme) const override {
                setColor(sf::Color(
                    (unsigned int) json["themes"][theme]["color-background-frame"][0],
                    (unsigned int) json["themes"][theme]["color-background-frame"][1],
                    (unsigned int) json["themes"][theme]["color-background-frame"][2],
                    (unsigned int) json["themes"][theme]["color-background-frame"][3]
                ));
            }
        public:
            Frame() {
                std::ifstream file {"build/config/display.json"};
                file >> json;
                file.close();

                setSize(sf::Vector2u(
                    (unsigned int) json["size"]["board"][0],
                    (unsigned int) json["size"]["board"][1]
                ));

                setScale({1.f, 1.f});

                setRotation(0.f);

                setPosition(sf::Vector2f(
                    (float) json["position"]["frame"][0],
                    (float) json["position"]["frame"][1]
                ));
            }

            void append(SampleObject* object) {
                frame.push_back(object);

                object->setStorage(this);
                object->window = window;

                handleAppend();
            }

            std::size_t size() const {
                return frame.size();
            }

            bool empty() const {
                return frame.empty();
            }

            
            const std::vector<SampleObject*>& objects() const { 
                return frame; 
            }
    };
}

#endif
