//
// Created by rhermes on 9/7/22.
//

#include "App.h"
#include "LTK/Shader.h"
#include "LTK/Program.h"
#include "stb/stb_image.h"
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

struct VertexData {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoords;

    static void registerVertexAttributes() {
        glVertexAttribPointer(0, decltype(pos)::length(), GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<const GLvoid*>(offsetof(VertexData, pos)));
        glVertexAttribPointer(1, decltype(color)::length(), GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<const GLvoid*>(offsetof(VertexData, color)));
        glVertexAttribPointer(2, decltype(texCoords)::length(), GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<const GLvoid*>(offsetof(VertexData, texCoords)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
};

void App::run() {
    createWindow();

    auto vertexShader = LTK::Shader::loadFromDisk(LTK::Shader::Type::Vertex, "data/shaders/default.vert");
    auto fragmentShader = LTK::Shader::loadFromDisk(LTK::Shader::Type::Fragment, "data/shaders/default.frag");
    LTK::Program shaderProgram({std::cref(vertexShader), std::cref(fragmentShader)});

    auto rawVertices = std::to_array({
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    });

    std::vector<VertexData> vertices;
    for (int i = 0; i < rawVertices.size(); i += 5) {
        vertices.emplace_back(
                glm::vec3{rawVertices[i], rawVertices[i+1], rawVertices[i+2]},
                glm::vec3{0.0f, 0.0f, 0.0f},
                glm::vec2{rawVertices[i+3], rawVertices[i+4]});
    }

    std::vector<GLuint> indices = {
            0, 1, 3, // First triangle
            1, 2, 3 // Second triangle
    };


    GLuint texture1;
    {
        // Texture generation
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char *data = stbi_load("data/textures/container.jpg", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error("Failed to load texture");
        }

        stbi_image_free(data);
    }

    GLuint texture2;
    {
        //glActiveTexture(GL_TEXTURE1);
        // Texture generation
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);

        int width, height, nrChannels;
        unsigned char *data = stbi_load("data/textures/awesomeface.png", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error("Failed to load texture");
        }

        stbi_image_free(data);
    }


    std::array<GLuint, 2> VAOs{};
    glGenVertexArrays(VAOs.size(), VAOs.data());

    std::array<GLuint, 2> VBOs{};
    glGenBuffers(VBOs.size(), VBOs.data());

    std::array<GLuint, 2> EBOs{};
    glGenBuffers(EBOs.size(), EBOs.data());

    // We only need one EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    for (int i = 0; i < 1; i++) {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);


        using T = decltype(vertices)::value_type;
        T::registerVertexAttributes();
    }


    // set the sampling thing
    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.87,0.11,0.72, 1.0);

    std::vector<glm::vec3> cubePositions = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    const double startTime = glfwGetTime();
    double prevTime = startTime;
    while (!glfwWindowShouldClose(window)) {
        const double currentTime = glfwGetTime();
        const double deltaTime = currentTime - prevTime;
        const double secondsSinceStart = currentTime - startTime;


        // Rendering loop
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shaderProgram.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::perspectiveFov(glm::radians(45.0f),
                                                static_cast<float>(windowWidth),
                                                static_cast<float>(windowHeight),
                                                0.1f, 100.0f);
        glm::mat4 vp = projection * view;


        for (int j = 0; j < 1; j++) {
            glBindVertexArray(VAOs[j]);
            for (int i = 0; i < cubePositions.size(); i++) {
                glm::mat4 model(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shaderProgram.setMat4("mvp", vp * model);
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            }
            // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

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
