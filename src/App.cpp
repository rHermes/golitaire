//
// Created by rhermes on 9/7/22.
//

#include "App.h"
#include "glfwpp/WindowBuilder.h"
#include <fmt/core.h>
#include <spdlog/spdlog.h>


using namespace gol;


App::App() : glfwCtx_(App::glfwErrorCallback_) {
    glfwWindow_ = std::make_unique<glfwpp::Window>(glfwpp::WindowBuilder()
            .setWindowSize(windowWidth_, windowHeight_)
            .setDebug(wantGlDebug)
            .build());

    if (wantGlDebug) {
        glfwWindow_->enableGLDebug(App::glKHRDebugOutputCallback, this);
    }

    game.resizeViewport(windowWidth_, windowHeight_);

    glfwWindow_->setWindowUserPointer(this);
    glfwWindow_->setFramebufferCallback(App::glfwFramebufferSizeCallback_);
    glfwWindow_->setKeyCallback(App::glfwKeyCallback_);
    glfwWindow_->setMousePositionCallback(App::glfwCursorPositionCallback_);
    glfwWindow_->setMouseButtonCallback(App::glfwMouseButtonCallback_);
}


void App::run() {
    game.init();

    spdlog::set_level(spdlog::level::debug);
    const double startTime = glfwCtx_.getTime();
    double prevTime = startTime;
    while (!glfwWindow_->windowShouldClose()) {
        // Update time keeping variables.
        const double currentTime = glfwGetTime();
        const double deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        glfwCtx_.pollEvents();

        game.processInput(deltaTime);
        game.update(deltaTime);
        game.render();

        glfwWindow_->swapBuffers();
    }
}

void App::handleKeyInput(int key, int scancode, int action, int mods) {
    // fmt::print("We got a key input event\n");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwWindow_->setWindowShouldClose(true);
    }

    if (glad_glPolygonModeNV) {

        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            glPolygonModeNV(GL_FRONT_AND_BACK, GL_LINE_NV);
        }

        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            glPolygonModeNV(GL_FRONT_AND_BACK, GL_FILL_NV);
        }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        game.restartGame();
    }

}


void App::handleFramebufferSizeEvent(int width, int height) {
    spdlog::debug("The framebuffer has been resized {}x{}!", width, height);
    windowWidth_ = width;
    windowHeight_ = height;

    game.resizeViewport(width, height);
}

void App::glfwKeyCallback_(GLFWwindow *window, const int key, const int scancode, const int action, const int mods) {
    App *app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app) {
        spdlog::error("Window app pointer is null, cannot execute key callback!");
        return;
    }

    app->handleKeyInput(key, scancode, action, mods);
}

void App::glfwCursorPositionCallback_(GLFWwindow* window, const double x, const double y) {
    App *app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app) {
        spdlog::error("Window app pointer is null, cannot execute mouse position callback!");
        return;
    }

    app->handleMousePositionEvent(x, y);
}

void App::glfwFramebufferSizeCallback_(GLFWwindow *window, int width, int height) {
    App *app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app) {
        spdlog::error("Window app pointer is null, cannot execute buffersize callback!");
        return;
    }

    app->handleFramebufferSizeEvent(width, height);
}

void App::glfwErrorCallback_(int code, const char *description) {
    spdlog::error("We got an error from glfw: {} {}", code, description);
}

void App::glKHRDebugOutputCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                   const char *message, const void *userParam) {
    const App *app = reinterpret_cast<const App*>(userParam);
    if (!app) {
        spdlog::error("Window app pointer is null, cannot execute KHR debug!");
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

void App::handleMousePositionEvent(const double x, const double y) {
    // we have to convert this to the actual coordinates.
    game.setMousePosition(static_cast<float>(x), static_cast<float>(windowHeight_ - y));
}

void App::glfwMouseButtonCallback_(GLFWwindow *window, int button, int action, int mods) {
    App *app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    if (!app) {
        spdlog::error("Window app pointer is null, cannot execute mouse button callback!");
        return;
    }

    app->handleMouseButtonEvent(button, action, mods);
}

void App::handleMouseButtonEvent(const int button, const int action, const int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            game.setMouseButtonState(MouseButton::Left, action == GLFW_PRESS); break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            game.setMouseButtonState(MouseButton::Right, action == GLFW_PRESS); break;
        default:
            break;
    }
}



