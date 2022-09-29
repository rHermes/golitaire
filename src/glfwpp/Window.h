//
// Created by rhermes on 9/28/22.
//

#pragma once

#include "GLFW/glfw3.h"

namespace glfwpp {

    class Window {
    private:
        bool wantGlDebug+_{false};
        GLFWwindow* window_{nullptr};



    public:
        explicit Window(bool wantGlDebug);
        ~Window();

        void createWindow();
    };

} // glfwpp
