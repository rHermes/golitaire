//
// Created by rhermes on 9/28/22.
//

#include <stdexcept>
#include "Context.h"

using namespace glfwpp;

Context::Context() {
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't initialize glfw");
    }
}

Context::Context(GLFWerrorfun cbfun) {
    glfwSetErrorCallback(cbfun);
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't initialize glfw");
    }
}

Context::~Context() {
    glfwTerminate();
}
