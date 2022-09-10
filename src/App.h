//
// Created by rhermes on 9/7/22.
//

#pragma once


#include <glad/gles2.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace gol {

class App {

private:
    const bool wantGlDebug = true;
    bool useGlDebug = false;

    static void glfwErrorCallback_(int code, const char* description);
    static void glfwKeyCallback_(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwFramebufferSizeCallback_(GLFWwindow* window, int width, int height);

    static void APIENTRY glKHRDebugOutputCallback(
            GLenum source, GLenum type, unsigned int id, GLenum severity,
            GLsizei length, const char *message, const void *userParam
            );


    [[nodiscard]] std::string readFile(std::string filename);
    [[nodiscard]] GLuint createShader(GLenum shaderType, std::string filename);
    [[nodiscard]] GLuint createProgram(const std::vector<GLuint>& shaders);

    void handleKeyInput(int key, int scancode, int action, int mods);
    void handleFramebufferSizeEvent(int width, int height);
    void handleKHRDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                              GLsizei length, const char *message) const;

    int windowWidth{640};
    int windowHeight{480};

    GLFWwindow* window;

    void createWindow();

public:

    App();
    ~App();

    void run();
};

} // gol
