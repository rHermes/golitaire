//
// Created by rhermes on 9/28/22.
//

#include "Window.h"

using namespace glfwpp;

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

Window::Window(bool wantGlDebug) {

}
