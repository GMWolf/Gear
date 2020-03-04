//
// Created by felix on 04/03/2020.
//

#ifndef GEAR_DEBUGUI_H
#define GEAR_DEBUGUI_H

#include <vector>

struct GLFWwindow;
namespace gear::ui {
    extern bool demoWindowOpen;

    void initialize(GLFWwindow* window);

    void begin();

    void end();

    void cleanup();

    struct PerfData {
        std::vector<float> plot;
        int maxCount = 60;
        double frameTime;
    };

    void perfWindow(PerfData& data);

}

#endif //GEAR_DEBUGUI_H
