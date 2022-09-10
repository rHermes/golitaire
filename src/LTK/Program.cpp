//
// Created by rhermes on 9/10/22.
//

#include <vector>
#include "Program.h"

using namespace LTK;

Program::Program(GLuint ID) : ID_{ID} {
}

void Program::use() const {
    glUseProgram(ID_);
}

Program::Program(std::span<std::reference_wrapper<const Shader>> shaders) {
    ID_ = glCreateProgram();

    for (const auto& shader : shaders) {
        attach(shader);
    }

    link();
}

Program::Program(std::initializer_list<std::reference_wrapper<const Shader>> shaders) {
    ID_ = glCreateProgram();

    for (const auto& shader : shaders) {
        attach(shader);
    }

    link();
}


void Program::link() { // NOLINT(readability-make-member-function-const)
    glLinkProgram(ID_);

    GLint success;
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        GLint bufferSize;
        glGetProgramiv(ID_, GL_INFO_LOG_LENGTH, &bufferSize);

        std::vector<GLchar> buffer(bufferSize + 1);
        glGetProgramInfoLog(ID_, static_cast<GLsizei>(buffer.size()), nullptr, buffer.data());

        fmt::print("ERROR::SHADER::PROGRAM::LINKING_FAILED:\n{}\n", buffer.data());
        throw std::runtime_error("Shader Program linking failed");
    }
}

void Program::attach(const Shader &shader) { // NOLINT(readability-make-member-function-const)
    glAttachShader(ID_, shader.id());
}

Program::~Program() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
    }
}

void Program::setBool(const std::string &name, bool value) { // NOLINT(readability-make-member-function-const)
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform1i(idx, value);
    }
}

void Program::setInt(const std::string &name, int value) { // NOLINT(readability-make-member-function-const)
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform1i(idx, value);
    }
}

void Program::setFloat(const std::string &name, float value) { // NOLINT(readability-make-member-function-const)
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform1f(idx, value);
    }
}
