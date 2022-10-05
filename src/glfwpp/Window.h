//
// Created by rhermes on 9/28/22.
//

#pragma once

#define GLFW_INCLUDE_NONE 1
#include "GLFW/glfw3.h"

namespace glfwpp {

    class Window {
    private:
        bool wantGlDebug_{false};
        GLFWwindow* window_{nullptr};

        int width{800};
        int height{600};


    public:
        explicit Window(bool wantGlDebug);
        ~Window();

        void createWindow();
    };

} // glfwpp
