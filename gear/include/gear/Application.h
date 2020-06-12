//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_APPLICATION_H
#define GEAR_APPLICATION_H

#include <string>
#include "KeyboardKeys.h"
#include <glm/vec2.hpp>

struct GLFWwindow;
namespace gear {

    struct AppConfig {
        int width = 640, height = 480;
        std::string title = "";
    };

    class ApplicationAdapter;

    class Application {
    public:
        explicit Application(const AppConfig &config);

        ~Application();

        void run(ApplicationAdapter &adapter);

        [[nodiscard]] bool keyPressed(KEYS key) const;

        [[nodiscard]] bool mousePressed(int mouseButton) const;

        glm::vec2 mousePosition() const;

        operator bool();

        int width, height;
        bool initialized = false;
        GLFWwindow *window{};

        double frameTime = 0;
    };

    void run(const AppConfig &config, ApplicationAdapter &app);

}


#endif //GEAR_APPLICATION_H
