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


#ifdef GLAD_DEBUG
void post_call_callback(const char *name, void *funcptr, int len_args, ...) {
    GLenum err;
    while((err = glad_glGetError()) != GL_NO_ERROR) {

        std::string errorName;
#define ERROR_NAME_CASE(x) case x : errorName = #x; break;

        switch (err) {
            ERROR_NAME_CASE(GL_INVALID_ENUM)
            ERROR_NAME_CASE(GL_INVALID_VALUE)
            ERROR_NAME_CASE(GL_INVALID_OPERATION)
            ERROR_NAME_CASE(GL_OUT_OF_MEMORY)
            ERROR_NAME_CASE(GL_INVALID_FRAMEBUFFER_OPERATION)
            default: errorName = "<Unknown error>";
        }

#undef ERROR_NAME_CASE
        std::cerr << errorName << " in " << name << "\n";
    }

}
#endif //GLAD_DEBUG

gear::Application::Application(const AppConfig& config) {
    glfwSetErrorCallback(glfw_error_callback);
#ifdef GLAD_DEBUG
    glad_set_post_callback(post_call_callback);
#endif //GLAD_DEBUG

    if (!glfwInit()) {
        std::cerr << "Could not initialize glfw\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);

    if (!window) {
        std::cerr << "Could not create window\n";
        glfwTerminate();
        return;
    }

    width = config.width;
    height = config.height;

    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, glfw_key_callback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize opengl context\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
    glfwSwapInterval(1);

    initialized = true;
}

void gear::Application::run(gear::ApplicationAdapter& adapter) {
    if (initialized) {
        glViewport(0, 0, width, height);

        adapter.init(this);

        while(!glfwWindowShouldClose(window)) {
            adapter.update();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        adapter.end();
    }
}

gear::Application::operator bool() {
    return initialized;
}

gear::Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool gear::Application::keyPressed(KEYS key) {
    return glfwGetKey(window, (int)key) == GLFW_PRESS;
}
