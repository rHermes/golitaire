//
// Created by rhermes on 9/7/22.
//

#include "App.h"
#include <fmt/core.h>

using namespace gol;


App::App() : glfwCtx_(App::glfwErrorCallback_){
}

App::~App() {
    glfwDestroyWindow(window);
}

void App::run() {
    createWindow();

    game.init();

    const double startTime = glfwGetTime();
    double prevTime = startTime;
    while (!glfwWindowShouldClose(window)) {
        // Update time keeping variables.
        const double currentTime = glfwGetTime();
        const double deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        glfwPollEvents();

        game.processInput(deltaTime);
        game.update(deltaTime);
        game.render();

        glfwSwapBuffers(window);
    }
}

void App::handleKeyInput(int key, int scancode, int action, int mods) {
    // fmt::print("We got a key input event\n");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glad_glPolygonModeNV) {

        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            glPolygonModeNV(GL_FRONT_AND_BACK, GL_LINE_NV);
        }

        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            glPolygonModeNV(GL_FRONT_AND_BACK, GL_FILL_NV);
        }
    }

}

void App::createWindow() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_CONTEXT_DEBUG, wantGlDebug);

    window = glfwCreateWindow(windowWidth, windowHeight, "Golitaire", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Couldn't create window");
    }

    glfwSetWindowUserPointer(window, this);


    glfwMakeContextCurrent(window);

    int version = gladLoadGLES2(glfwGetProcAddress);
    if (version == 0) {
        fmt::print("Failed to initialize OpenGL ES\n");
        throw std::runtime_error("Failed to initialize OpenGL ES");
    }

    fmt::print("Loaded OpenGL ES: {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    if (wantGlDebug) {
        useGlDebug = GLAD_GL_KHR_debug;
        if (useGlDebug) {
            fmt::print("KHR debug is supported!\n");

            glEnable(GL_DEBUG_OUTPUT_KHR);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);

            glDebugMessageCallbackKHR(App::glKHRDebugOutputCallback, this);
            glDebugMessageControlKHR(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        } else {
            fmt::print("GL debug wanted, but not supported\n");
        }
    }

    glfwSwapInterval(1);

    game.resizeViewport(windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, App::glfwFramebufferSizeCallback_);
    glfwSetKeyCallback(window, App::glfwKeyCallback_);
}





void App::handleFramebufferSizeEvent(int width, int height) {
    fmt::print("The framebuffer has been resized {}x{}!\n", width, height);
    windowWidth = width;
    windowHeight = height;

    game.resizeViewport(width, height);
}

void App::glfwKeyCallback_(GLFWwindow *window, int key, int scancode, int action, int mods) {
    App *app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app) {
        fmt::print("Window app pointer is null!\n");
        return;
    }

    app->handleKeyInput(key, scancode, action, mods);
}

void App::glfwFramebufferSizeCallback_(GLFWwindow *window, int width, int height) {
    App *app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app) {
        fmt::print("Window app pointer is null!\n");
        return;
    }

    app->handleFramebufferSizeEvent(width, height);
}

void App::glfwErrorCallback_(int code, const char *description) {
    fmt::print("We got an error glfw_cpp: {} {}\n", code, description);
}

void App::glKHRDebugOutputCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                   const char *message, const void *userParam) {
    const App *app = reinterpret_cast<const App*>(userParam);
    if (!app) {
        fmt::print("app pointer is null!\n");
        return;
    }

    app->handleKHRDebugOutput(source, type, id, severity, length, message);
}

void App::handleKHRDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message) const {

    switch (id) {
        case 131185:
            return;
    }
    fmt::print("---------------\n");
    fmt::print("Debug message ({}): {}\n", id, message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API_KHR:             fmt::print("Source: API\n"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR:   fmt::print("Source: Window System\n"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_KHR: fmt::print("Source: Shader Compiler\n"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_KHR:     fmt::print("Source: Third Party\n"); break;
        case GL_DEBUG_SOURCE_APPLICATION_KHR:     fmt::print("Source: Application\n"); break;
        case GL_DEBUG_SOURCE_OTHER_KHR:           fmt::print("Source: Other\n"); break;
        default: fmt::print("Source: SHOULD NEVER HAPPEN"); break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR_KHR:               fmt::print("Type: Error\n"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR: fmt::print("Type: Deprecated Behaviour\n"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR:  fmt::print("Type: Undefined Behaviour\n"); break;
        case GL_DEBUG_TYPE_PORTABILITY_KHR:         fmt::print("Type: Portability\n"); break;
        case GL_DEBUG_TYPE_PERFORMANCE_KHR:         fmt::print("Type: Performance\n"); break;
        case GL_DEBUG_TYPE_MARKER_KHR:              fmt::print("Type: Marker\n"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP_KHR:          fmt::print("Type: Push Group\n"); break;
        case GL_DEBUG_TYPE_POP_GROUP_KHR:           fmt::print("Type: Pop Group\n"); break;
        case GL_DEBUG_TYPE_OTHER_KHR:               fmt::print("Type: Other\n"); break;
        default: fmt::print("Type: SHOULD NEVER HAPPEN"); break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH_KHR:         fmt::print("Severity: high\n"); break;
        case GL_DEBUG_SEVERITY_MEDIUM_KHR:       fmt::print("Severity: medium\n"); break;
        case GL_DEBUG_SEVERITY_LOW_KHR:          fmt::print("Severity: low\n"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION_KHR: fmt::print("Severity: notification\n"); break;
        default: fmt::print("Severity: SHOULD NEVER HAPPEN"); break;
    }
}

