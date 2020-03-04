//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_APPLICATION_H
#define GEAR_APPLICATION_H

#include <string>
#include "ApplicationAdapter.h"
#include "KeyboardKeys.h"

struct GLFWwindow;
namespace gear {

    struct AppConfig {
        int width = 640, height = 480;
        std::string title = "";
    };

    class Application {
    public:
        explicit Application(const AppConfig &config);

        ~Application();

        void run(ApplicationAdapter &adapter);

        bool keyPressed(KEYS key);

        operator bool();


        int width, height;
        bool initialized = false;
        GLFWwindow *window{};
    };

    void run(const AppConfig &config, ApplicationAdapter &app);

}


#endif //GEAR_APPLICATION_H
