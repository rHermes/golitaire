//
// Created by rhermes on 9/7/22.
//

#include "Context.h"

using namespace glfw_cpp;

Context::Context() {
    if(!glfwInit()) {
        throw std::runtime_error("We couldn't start glfw_cpp");
    }
}

Context::~Context() {
    glfwTerminate();
}
