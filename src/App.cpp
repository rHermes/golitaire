//
// Created by rhermes on 9/7/22.
//

#include "App.h"
#include "LTK/Shader.h"
#include "LTK/Program.h"
#include <fmt/core.h>

#include <fstream>
#include <sstream>
#include <vector>

#include <filesystem>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace gol;


App::App() {
    glfwSetErrorCallback(App::glfwErrorCallback_);
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't initialize glfw");
    }
}

App::~App() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void App::run() {
    createWindow();

    auto vertexShader = LTK::Shader::loadFromDisk(LTK::Shader::Type::Vertex, "data/shaders/default.vert");
    auto fragmentShader = LTK::Shader::loadFromDisk(LTK::Shader::Type::Fragment, "data/shaders/default.frag");
    LTK::Program shaderProgram({std::cref(vertexShader), std::cref(fragmentShader)});


    const auto vertices = std::to_array({
        glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),  // top right
        glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), // bottom right
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),  // bottom left
        glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),  // top left
    });

    std::array<std::remove_cv_t<decltype(vertices)>,2> verts{};
    for (int i = 0; i < vertices.size(); i += 2) {
        auto t = vertices[i] * 0.5f;
        verts[0][i] = t + glm::vec3(-0.5, 0, 0);
        verts[0][i+1] = vertices[i+1];
        verts[1][i] = t + glm::vec3(0.5, 0, 0);
        verts[1][i+1] = vertices[i+1];
    }

    auto indices = std::to_array<unsigned int>({  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    });


    std::array<GLuint, 2> VAOs{};
    glGenVertexArrays(VAOs.size(), VAOs.data());

    std::array<GLuint, 2> VBOs{};
    glGenBuffers(VBOs.size(), VBOs.data());

    std::array<GLuint, 2> EBOs{};
    glGenBuffers(EBOs.size(), EBOs.data());

    // We only need one EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    for (int i = 0; i < verts.size(); i++) {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, verts[i].size() * sizeof(verts[i][0]), verts[i].data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);

        glVertexAttribPointer(0, verts[i][0].length(), GL_FLOAT, GL_FALSE, 2*sizeof(verts[i][0]), nullptr);
        glVertexAttribPointer(1, verts[i][0].length(), GL_FLOAT, GL_FALSE, 2*sizeof(verts[i][0]),
                              reinterpret_cast<const void *>(sizeof(verts[i][0])));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }



    glClearColor(0.87,0.11,0.72, 1.0);

    const double startTime = glfwGetTime();
    double prevTime = startTime;
    while (!glfwWindowShouldClose(window)) {
        const double currentTime = glfwGetTime();
        const double deltaTime = currentTime - prevTime;
        const double secondsSinceStart = currentTime - startTime;

        // Rendering loop
        glClear(GL_COLOR_BUFFER_BIT);

        float greenValue = static_cast<float>(std::sin(secondsSinceStart)) / 2.0f + 0.5f;

        shaderProgram.use();
        for (int i = 0; i < VAOs.size(); i++) {
            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
        prevTime = currentTime;
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

    // glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, App::glfwFramebufferSizeCallback_);
    glfwSetKeyCallback(window, App::glfwKeyCallback_);
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

void App::handleFramebufferSizeEvent(int width, int height) {
    fmt::print("The framebuffer has been resized {}x{}!\n", width, height);
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);
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

std::string App::readFile(std::string filename) {
    std::ifstream ifs(filename, std::ios_base::in);
    if (!ifs.is_open()) {
        throw std::runtime_error("None existant file: " + filename);
    }
    std::stringstream strStream;

    strStream << ifs.rdbuf();
    return strStream.str();
}

GLuint App::createShader(GLenum shaderType, std::string filename) {

    std::filesystem::path cwd = std::filesystem::current_path();

    std::string src = readFile(filename);

    GLuint shader = glCreateShader(shaderType);
    if (useGlDebug) {
        glObjectLabelKHR(GL_SHADER_KHR, shader, -1, filename.c_str());
    }

    auto ww = src.c_str();
    glShaderSource(shader, 1, &ww, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint bufferSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufferSize);

        std::vector<GLchar> buffer(bufferSize + 1);
        glGetShaderInfoLog(shader, buffer.size(), nullptr, buffer.data());

        fmt::print("Failed to compile shader {}:\n{}\n", filename, buffer.data());

        throw std::runtime_error("Shader compilation failed");
    }

    return shader;
}

GLuint App::createProgram(const std::vector<GLuint> &shaders) {
    GLuint program = glCreateProgram();
    for (const auto& shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint bufferSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufferSize);

        std::vector<GLchar> buffer(bufferSize + 1);
        glGetProgramInfoLog(program, buffer.size(), nullptr, buffer.data());

        fmt::print("Failed to link program:\n{}\n", buffer.data());

        throw std::runtime_error("Shader Program linking failed");
    }

    return program;
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
