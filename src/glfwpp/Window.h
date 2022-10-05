//
// Created by rhermes on 9/28/22.
//

#pragma once

#define GLFW_INCLUDE_NONE 1
#include "GLFW/glfw3.h"
#include "glad/gles2.h"

namespace glfwpp {

    class Window {
    private:
        GLFWwindow* window_{nullptr};


    public:
        explicit Window(GLFWwindow* window);
        ~Window();

        // We don't want copying or moving
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        void setWindowUserPointer(void* userdata);
        void setFramebufferCallback(GLFWframebuffersizefun cb);
        void setKeyCallback(GLFWkeyfun cb);


        void enableGLDebug(GLDEBUGPROCKHR cb, void* userdata);

        void setWindowShouldClose(bool should);
        [[nodiscard]] bool windowShouldClose() const;
        void swapBuffers();
    };

} // glfwpp
