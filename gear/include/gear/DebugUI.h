//
// Created by felix on 04/03/2020.
//

#ifndef GEAR_DEBUGUI_H
#define GEAR_DEBUGUI_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace gear::ui {
    extern bool demoWindowOpen;

    void initialize(GLFWwindow* window);

    void begin();

    void end();

    void cleanup();

}

#endif //GEAR_DEBUGUI_H
