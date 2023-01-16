//
// Created by rhermes on 9/28/22.
//

#include <stdexcept>
#include <utility>

#include "Window.h"

#include <glad/gles2.h>
#include <spdlog/spdlog.h>

using namespace glfwpp;

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

Window::Window(GLFWwindow *window) : window_{window} {
    glfwMakeContextCurrent(window_);

    int version = gladLoadGLES2(glfwGetProcAddress);
    if (version == 0) {
        throw std::runtime_error("Failed to initialize OpenGL ES");
    }

    spdlog::info(
            "Loaded OpenGL ES: {}.{}",
           GLAD_VERSION_MAJOR(version),
           GLAD_VERSION_MINOR(version)
            );

    glfwSwapInterval(1);
}

void Window::enableGLDebug(GLDEBUGPROCKHR cb, void* userdata) {
    if (GLAD_GL_KHR_debug) {
        glEnable(GL_DEBUG_OUTPUT_KHR);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);

        glDebugMessageCallbackKHR(cb, userdata);
        glDebugMessageControlKHR(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    } else {
        spdlog::error("GL debug wanted, but it's not supported");
    }
}

void Window::setWindowUserPointer(void *userdata) {
    glfwSetWindowUserPointer(window_, userdata);
}

void Window::setFramebufferCallback(GLFWframebuffersizefun cb) {
    glfwSetFramebufferSizeCallback(window_, cb);
}

void Window::setKeyCallback(GLFWkeyfun cb) {
    glfwSetKeyCallback(window_, cb);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window_);
}

bool Window::windowShouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::setWindowShouldClose(bool should) {
    glfwSetWindowShouldClose(window_, should);
}

Window::Window(Window &&other) noexcept {
    *this = std::move(other);
}

Window &Window::operator=(Window &&other) noexcept {
    window_ = std::exchange(other.window_, nullptr);
    return *this;
}

void Window::setMousePositionCallback(GLFWcursorposfun cb) {
    glfwSetCursorPosCallback(window_, cb);
}


