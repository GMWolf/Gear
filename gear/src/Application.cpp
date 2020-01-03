//
// Created by felix on 03/01/2020.
//

#include <gear/Application.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW error: " << description << "\n";
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void gear::run(const AppConfig& config, ApplicationAdapter& app) {

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        std::cerr << "Could not initialize glfw\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    int width = 640, height = 480;
    GLFWwindow* window = glfwCreateWindow(width, height, "Gear", nullptr, nullptr);

    if (!window) {
        std::cerr << "Could not create window\n";
        glfwTerminate();
        return;
    }

    glfwSetKeyCallback(window, glfw_key_callback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize opengl context\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
    glfwSwapInterval(1);

    glViewport(0, 0, width, height);

    app.init();

    while(!glfwWindowShouldClose(window)) {
        app.update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return;
}