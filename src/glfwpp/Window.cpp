//
// Created by rhermes on 9/28/22.
//

#include <stdexcept>
#include "Window.h"

#include <glad/gles2.h>
#include <spdlog/spdlog.h>

using namespace glfwpp;

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

Window::Window(bool wantGlDebug) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_CONTEXT_DEBUG, wantGlDebug);

    window_ = glfwCreateWindow(width, height, "Golitaire", nullptr, nullptr);
    if (!window_) {
        throw std::runtime_error("Couldn't create window");
    }

    glfwSetWindowUserPointer(window_, this);

    int version = gladLoadGLES2(glfwGetProcAddress);
    if (version == 0) {
        throw std::runtime_error("Failed to initialize OpenGL ES");
    }

    spdlog::info("wow {}", 3);
    spdlog::info(
            "Loaded OpenGL ES: {}.{}", 2, 3
           // GLAD_VERSION_MAJOR(version),
           // GLAD_VERSION_MINOR(version)
            );
}
