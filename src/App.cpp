//
// Created by rhermes on 9/7/22.
//

#include "App.h"
#include <fmt/core.h>

using namespace gol;

void App::glfwErrorCallback_(int code, const char *description) {
    fmt::print("We got an error glfw_cpp: {} {}\n", code, description);
}

App::App() {
    glfwSetErrorCallback(App::glfwErrorCallback_);
}

void App::run() {

}
