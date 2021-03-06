//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_APPLICATION_H
#define GEAR_APPLICATION_H

#include <string>
#include "KeyboardKeys.h"
#include <glm/vec2.hpp>
#include <memory>
#include <gear/gapi.h>

struct GLFWwindow;
namespace gear {

    struct AppConfig {
        int width = 640, height = 480;
        std::string title;
        const Gapi* gapi;
    };

    class ApplicationAdapter;
    class InputState;

    class Application {

        std::unique_ptr<InputState> inputState;

    public:
        explicit Application(const AppConfig &config);

        ~Application();

        void run(ApplicationAdapter &adapter);

        [[nodiscard]] bool mousePressed(int mouseButton) const;

        [[nodiscard]] glm::vec2 mousePosition() const;

        void close();

        int width, height;
        bool initialized = false;
        GLFWwindow *window{};

        InputState& getInputState();

        double frameTime = 0;
    };
}


#endif //GEAR_APPLICATION_H
