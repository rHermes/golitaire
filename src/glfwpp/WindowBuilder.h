//
// Created by rhermes on 10/5/22.
//

#pragma once

#include "Window.h"

namespace glfwpp {

    class WindowBuilder {
    private:
        bool glDebug_{false};

        int width_{800};
        int height_{640};

    public:

        WindowBuilder& setDebug(bool enabled);
        WindowBuilder& setWidth(int width);
        WindowBuilder& setHeight(int height);
        WindowBuilder& setWindowSize(int width, int height);


        [[nodiscard]] Window build();
    };

} // LTK
