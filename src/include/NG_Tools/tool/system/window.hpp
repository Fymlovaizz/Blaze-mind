#ifndef __TOOL_SYSTEM_WINDOW_HPP__
#define __TOOL_SYSTEM_WINDOW_HPP__

namespace ng {
    class Window {
        private:
            sf::RenderWindow                renderWindow;
            int                             theme;
            std::string                     nameTheme;
            nlohmann::json                  displayJson;
            nlohmann::json                  launchJson;

            sf::Music                       music;

            sf::Color                       backgroundColor;
            sf::Event                       event;

            sf::Clock                       clock;
            sf::Time                        currentTime = clock.getElapsedTime();
            sf::Time                        lastTime = currentTime;
            float                           deltatime = 0.f;

            unsigned int                    fps;
            float                           spf;
            float                           systemTime = 0.f;
            sf::Font                        systemFont;
            sf::Text                        systemText;
            nlohmann::json                  systemJson;

            bool                            isMouseActive = true;
            bool                            isMousePressed = false;
            unsigned char                   mouseCombo = 0;
            float                           mouseTime = 0.f;
            float                           mouseDelayTime = 0.5f;
            sf::Vector2f                    position = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
            sf::Vector2f                    location = {INT_MIN, INT_MIN};
            sf::Vector2f                    situation = position;
            sf::Vector2f                    deltapos = {};
            sf::Vector2f                    updatePosition = position;
            sf::Vector2f                    updateLocation = location;
            sf::Vector2f                    currentPosition = {0, 0};
            sf::Vector2f                    selectedPosition = {0, 0};
            sf::Vector2f                    previousPosition = {0, 0};

            std::vector<SampleObject*>      objectList;
            SampleObject*                   selected = nullptr;
            SampleObject*                   current = nullptr;
            SampleObject*                   previous = nullptr;

            bool                            needHandle = true;
            bool                            called = false;

            inline void processUpdate(sf::Vector2f update) {
                updatePosition = position + update;
                updateLocation = location + update;
            }

            inline void technique() {
                currentTime = clock.getElapsedTime();
                deltatime = (currentTime - lastTime).asSeconds();
                lastTime = currentTime;

                position = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
                deltapos = position - situation;
                situation = position;

                ++fps;
                systemTime -= deltatime;
                if (systemTime < 0.f) {
                    spf = (-systemTime + 1.f) / fps;
                    systemTime = 1.f;
                    systemText.setString(
                        "Theme: " + nameTheme +
                        ", FPS: " + std::to_string(1.f / spf) + 
                        ", SPF: " + std::to_string(spf)
                    );
                    fps = 0;
                }

                if (mouseTime > 0.f) {
                    mouseTime -= deltatime;
                    if (mouseTime <= 0.f) {
                        if (isMousePressed && selected != nullptr) {
                            processUpdate(selectedPosition);
                            selected->handleHold();
                        }
                        mouseCombo = 0;
                    }
                }
            }

            inline void input() {
                while (renderWindow.pollEvent(event)) {
                    switch (event.type) {
                        case sf::Event::Closed:
                        {
                            renderWindow.close();
                            break;
                        }
                        case sf::Event::KeyPressed:
                        {
                            if (event.key.code == sf::Keyboard::Escape) {
                                renderWindow.close();
                                break;
                            }
                            if (selected != nullptr) {
                                processUpdate(selectedPosition);
                                if (event.key.alt) {
                                    selected->handleAlt();
                                }
                                else if (event.key.control) {
                                    selected->handleCtrl();
                                }
                                else if (event.key.shift) {
                                    selected->handleShift();
                                }
                                else {
                                    selected->handleInput();
                                }
                            }
                            break;
                        }
                        case sf::Event::TextEntered:
                        {
                            if (selected != nullptr) {
                                processUpdate(selectedPosition);
                                selected->handleText();
                            }
                            break;
                        }
                        case sf::Event::MouseButtonPressed:
                        {
                            isMousePressed = true;
                            isMouseActive = true;
                            location = position;

                            if (selected != current) {
                                if (selected != nullptr) {
                                    processUpdate(selectedPosition);
                                    selected->handleExit();
                                }
                            }
                            if (current != nullptr) {
                                mouseTime = mouseDelayTime;
                                processUpdate(currentPosition);
                                current->handlePress();
                            }

                            selectedPosition = currentPosition;
                            selected = current;
                            break;
                        }
                        case sf::Event::MouseButtonReleased:
                        {
                            isMousePressed = false;

                            if (selected != nullptr) {
                                processUpdate(selectedPosition);
                                selected->handleRelease();
                                if (isMouseActive) {
                                    if (mouseTime > 0.f) {
                                        mouseTime = mouseDelayTime;
                                        ++mouseCombo;
                                        if (selected == current) {
                                            selected->handleClick();
                                        }
                                    }
                                }
                                else {
                                    selected->handleDrop();
                                }
                            }
                        }
                        case sf::Event::MouseWheelScrolled:
                        {
                            if (current != nullptr) {
                                processUpdate(currentPosition);
                                current->handleScroll();
                            }
                            break;
                        }
                        case sf::Event::MouseMoved:
                        {
                            if (!isMousePressed) {
                                if (current != nullptr) {
                                    processUpdate(currentPosition);
                                    current->handleMove();
                                }
                            }
                            else {
                                isMouseActive = false;
                                mouseTime = 0.f;
                                mouseCombo = 0;
                                if (selected != nullptr) {
                                    processUpdate(selectedPosition);
                                    selected->handleDrag();
                                }
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
            }

            inline void handle() {
                if (current != previous) {
                    if (previous != nullptr) {
                        processUpdate(previousPosition);
                        previous->handleLeave();
                    }
                    if (current != nullptr) {
                        processUpdate(currentPosition);
                        current->handleEnter();
                    }
                    previousPosition = currentPosition;
                    previous = current;
                }
            }

            bool process(SampleObject* object, bool inSide = true) {
                object->update();

                if (inSide) {
                    if (object->isWithinObject(updatePosition)) {
                        currentPosition = updatePosition - position;
                        current = object;
                    }
                    else {
                        inSide = false;
                    }
                }

                if (Frame* frame = dynamic_cast<Frame*>(object)) {
                    if (!frame->empty()) {
                        sf::Vector2f position_object = frame->getPosition();
                        updatePosition -= position_object;
                        for (SampleObject* object : frame->objects()) {
                            if (process(object, inSide)) {
                                object->needsRedraw = true;
                            }
                        }
                        updatePosition += position_object;
                    }
                }

                return object->needsRedraw;
            }

            inline void output() {
                updatePosition = position;
                updateLocation = location;

                currentPosition = {0, 0};
                current = nullptr;

                for (SampleObject* object : objectList) {
                    process(object);
                    object->draw(renderWindow);
                }

                handle();
            }

            inline bool setFile() {
                std::ifstream launchFile;
                std::ifstream displayFile;
                launchFile.open("build/config/launch.json");
                displayFile.open("build/config/display.json");

                if (!launchFile.is_open()) {
                    std::cerr << "Error: Can't open 'build/config/launch.json'!" << std::endl;
                    renderWindow.close();
                    return false;
                }
                if (!displayFile.is_open()) {
                    std::cerr << "Error: Can't open 'build/config/display.json'!" << std::endl;
                    renderWindow.close();
                    return false;
                }

                launchFile >> launchJson;
                displayFile >> displayJson;
                launchFile.close();
                displayFile.close();

                theme = (int) launchJson["theme"];
                for (SampleObject* object : objectList) {
                    object->handleSetTheme(theme);
                }

                if (theme >= displayJson["themes"].size()) {
                    renderWindow.close();
                    return false;
                }

                nameTheme = (std::string) displayJson["themes"][theme]["name"] + " (" + (std::string) displayJson["themes"][theme]["type"] + ')';
                return true;
            }

            inline void setFont() {
                if (!systemFont.loadFromFile(
                    (std::string) displayJson["paths"]["resource"] + 
                    (std::string) displayJson["paths"]["font"] + 
                    (std::string) displayJson["themes"][theme]["font-system"]
                )) {
                    std::cerr << "Warning: Can't load font '" << 
                    (std::string) displayJson["themes"][theme]["font-system"] 
                    << "' for 'Window'!" << std::endl;
                }
                else {
                    systemText.setFont(systemFont);
                    systemText.setCharacterSize(displayJson["size_font"]["s"]);
                    systemText.setFillColor(
                        sf::Color(
                            (unsigned char) displayJson["themes"][theme]["color-text-system"][0],
                            (unsigned char) displayJson["themes"][theme]["color-text-system"][1],
                            (unsigned char) displayJson["themes"][theme]["color-text-system"][2],
                            (unsigned char) displayJson["themes"][theme]["color-text-system"][3]
                        )
                    );

                    systemText.setPosition({10.f, 10.f});
                }
            }

            inline void setIcon() {
                sf::Image icon;
                if (!icon.loadFromFile(
                    (std::string) displayJson["paths"]["resource"] + 
                    (std::string) displayJson["paths"]["image"] + 
                    (std::string) displayJson["paths"]["images"]["logo"]
                )) {
                    std::cerr << "Warning: Can't load icon for 'Window'!" << std::endl;                    
                }
                else {
                    renderWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
                }
            }

            inline void setBackground() {
                backgroundColor = sf::Color(
                    (unsigned char) displayJson["themes"][theme]["color-background"][0],
                    (unsigned char) displayJson["themes"][theme]["color-background"][1],
                    (unsigned char) displayJson["themes"][theme]["color-background"][2],
                    (unsigned char) displayJson["themes"][theme]["color-background"][3]
                );
            }

            inline void setMusic() {
                if (!(bool) launchJson["music-theme"]) return;
                if (displayJson["themes"][theme].contains("music-background")) {
                    if (!music.openFromFile(
                        (std::string) displayJson["paths"]["resource"] + 
                        (std::string) displayJson["paths"]["music"] + 
                        (std::string) displayJson["themes"][theme]["music-background"]
                    )) {
                        std::cerr << "Warning: Can't load music '" << 
                        (std::string) displayJson["themes"][theme]["music-background"] 
                        << "' for 'Window'!" << std::endl;
                    }
                    else {
                        if (displayJson["themes"][theme].contains("music-volume")) {
                            music.setVolume((float) displayJson["themes"][theme]["music-volume"]);
                        }
                        else {
                            music.setVolume(10.f);
                        }
                        
                        music.setLoop(true);
                        music.play();
                    }
                }
            }

            inline void setWindow() {
                if ((bool) launchJson["fullscreen"]) {
                    renderWindow.create(sf::VideoMode::getDesktopMode(), "NG Tools", sf::Style::Fullscreen);
                }
                else {
                    renderWindow.create(sf::VideoMode::getDesktopMode(), "NG Tools", sf::Style::Default);
                }

                if ((bool) launchJson["limit-fps"]) renderWindow.setFramerateLimit(launchJson["fps-limit"]);
                renderWindow.setVerticalSyncEnabled((bool) launchJson["vertical-sync-enabled"]);
            }

            bool begin() {
                if (called) {
                    std::cerr << "Warning: \n\t- The command was not executed.\n\t- Already called 'mainloop()'!" << std::endl;
                    return true;
                }
                called = true;
                if (!setFile()) return true;
                setWindow();

                setFont();
                setIcon();
                setBackground();
                setMusic();

                return false;
            }

            void end() {
                called = false;
                objectList.clear();

                music.stop();
            }
        public:
            ~Window() = default;

            void mainloop() {
                if (begin()) return;

                while (renderWindow.isOpen()) {
                    renderWindow.clear(backgroundColor);

                    technique();
                    input();
                    output();

                    renderWindow.draw(systemText);
                    renderWindow.display();
                }

                end();
            }

            inline void append(SampleObject* object) {
                object->window = this;
                objectList.push_back(object);
            }

            inline void setSelected(SampleObject* object) {
                selected = object;
            }

            inline void setTheme(int theme) {
                if (theme >= displayJson["themes"].size()) {
                    return;
                }

                this->theme = theme;
                nameTheme = (std::string) displayJson["themes"][theme]["name"] + " (" + (std::string) displayJson["themes"][theme]["type"] + ')';

                setFont();
                setBackground();
                setMusic();

                for (SampleObject* object : objectList) {
                    object->handleSetTheme(theme);
                }
            }

            const sf::Vector2f& getMousePosition() { return updatePosition; }
            const sf::Vector2f& getMouseLocation() { return updateLocation; }
            const sf::Vector2f& getMouseDeltapos() { return deltapos; }

            const float getDeltatime() { return deltatime; }

            const sf::Event& getEvent() { return event; }

            const sf::View& getView() { return renderWindow.getView(); }
            const sf::View& getDefaultView() { return renderWindow.getDefaultView(); }
    };
}

#endif
