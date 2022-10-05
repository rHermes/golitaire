//
// Created by rhermes on 10/5/22.
//

#include "WindowBuilder.h"

using namespace glfwpp;

WindowBuilder &WindowBuilder::setDebug(bool enabled) {
    glDebug_ = enabled;
    return *this;
}

Window WindowBuilder::build() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_CONTEXT_DEBUG, glDebug_);

    GLFWwindow* window = glfwCreateWindow(width_, height_, "Golitaire", nullptr, nullptr);

    return Window(window);
}
