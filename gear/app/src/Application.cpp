//
// Created by felix on 03/01/2020.
//

#include <Application.h>
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Input.h>
#include <ApplicationAdapter.h>

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW error: " << description << "\n";
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* app = static_cast<gear::Application*>(glfwGetWindowUserPointer(window));
    app->getInputState().updateKey(static_cast<gear::KEYS>(key), static_cast<gear::KeyEvent>(action));
}


gear::Application::Application(const AppConfig& config) {
    glfwSetErrorCallback(glfw_error_callback);
#ifdef GLAD_DEBUG
    glad_set_post_callback(post_call_callback);
#endif //GLAD_DEBUG

    if (!glfwInit()) {
        std::cerr << "Could not initialize glfw\n";
        return;
    }

    inputState = std::make_unique<InputState>();

    if (auto ogl = std::get_if<Gapi::Ogl>(&config.gapi->api)) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ogl->version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ogl->version_minor);
    }

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
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    if (auto ogl = std::get_if<Gapi::Ogl>(&config.gapi->api)) {
        if (!ogl->loadGLLoader((Gapi::Ogl::LoadProcFn *) glfwGetProcAddress)) {
            std::cerr << "Failed to initialize opengl context\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }
    }
    glfwSwapInterval(0);

    initialized = true;
}



void gear::Application::run(gear::ApplicationAdapter& adapter) {
    if (initialized) {
        adapter.init(this);

        int frame = 1;
        while(!glfwWindowShouldClose(window)) {
            inputState->updateFrame(frame);
            glfwPollEvents();

            double time = glfwGetTime();

            adapter.update();

            glfwSwapBuffers(window);

            frameTime = glfwGetTime() - time;
            frame++;
        }

        adapter.end();
    }
}

gear::Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
bool gear::Application::mousePressed(int mouseButton) const {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
}

glm::vec2 gear::Application::mousePosition() const {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {x, height - y};
}

gear::InputState& gear::Application::getInputState() {
    return *inputState;
}

void gear::Application::close() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
