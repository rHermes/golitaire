//
// Created by rhermes on 9/7/22.
//

#pragma once


#include <glad/gles2.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "Game.h"
#include "glfwpp/Context.h"
#include "glfwpp/Window.h"

namespace gol {
/// The App is responsible for window and GLFW management. This is
/// the class that you would replace if you where porting this to
/// SDL
class App {

private:
    glfwpp::Context glfwCtx_;
    std::unique_ptr<glfwpp::Window> glfwWindow_;

    Game game;

    const bool wantGlDebug = true;
    bool useGlDebug = false;

    static void glfwErrorCallback_(int code, const char* description);
    static void glfwKeyCallback_(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwFramebufferSizeCallback_(GLFWwindow* window, int width, int height);
    static void glfwCursorPositionCallback_(GLFWwindow* window, double x, double y);
    static void glfwMouseButtonCallback_(GLFWwindow* window, int button, int action, int mods);


    static void APIENTRY glKHRDebugOutputCallback(
            GLenum source, GLenum type, unsigned int id, GLenum severity,
            GLsizei length, const char *message, const void *userParam
            );



    void handleMousePositionEvent(double x, double y);
    void handleMouseButtonEvent(int button, int action, int mods);
    void handleKeyInput(int key, int scancode, int action, int mods);
    void handleFramebufferSizeEvent(int width, int height);
    void handleKHRDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                              GLsizei length, const char *message) const;

    int windowWidth_{640};
    int windowHeight_{480};

public:

    App();

    void run();
};

} // gol
