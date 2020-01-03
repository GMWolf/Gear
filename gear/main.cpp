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

int main() {

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        std::cerr << "Could not initialize glfw\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    int width = 640, height = 480;
    GLFWwindow* window = glfwCreateWindow(width, height, "Gear", nullptr, nullptr);

    if (!window) {
        std::cerr << "Could not create window\n";
        glfwTerminate();
        return 1;
    }

    glfwSetKeyCallback(window, glfw_key_callback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize opengl context\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }
    glfwSwapInterval(1);

    glViewport(0, 0, width, height);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}