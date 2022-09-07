//
// Created by rhermes on 9/7/22.
//

#pragma once

#include <glad/gles2.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace gol {

class App {

private:

    static void glfwErrorCallback_(int code, const char* description);

public:

    App();

    void run();
};

} // gol
