//
// Created by rhermes on 9/28/22.
//

#pragma once

#include "GLFW/glfw3.h"

namespace glfwpp {

    class Context {
    public:
        Context();
        explicit Context(GLFWerrorfun cbfun);

        ~Context();

        // We don't want copying or moving
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        Context(Context&&) = delete;
        Context& operator=(Context&&) = delete;
    };

} // glfwpp
