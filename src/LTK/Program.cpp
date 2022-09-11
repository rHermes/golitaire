//
// Created by rhermes on 9/10/22.
//

#include <vector>
#include "Program.h"
#include "glm/gtc/type_ptr.hpp"

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


void Program::link() {
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

void Program::attach(const Shader &shader) {
    glAttachShader(ID_, shader.id());
}

Program::~Program() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
    }
}

void Program::setBool(const std::string &name, bool value) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform1i(idx, value);
    }
}

void Program::setInt(const std::string &name, int value) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform1i(idx, value);
    }
}

void Program::setFloat(const std::string &name, float value) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform1f(idx, value);
    }
}

void Program::setVec2(const std::string &name, const glm::vec2 &value) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform2fv(idx, 1, glm::value_ptr(value));
    }
}

void Program::setVec2(const std::string &name, float x, float y) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform2f(idx, x, y);
    }
}

void Program::setVec3(const std::string &name, const glm::vec3 &value) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform3fv(idx, 1, glm::value_ptr(value));
    }
}

void Program::setVec3(const std::string &name, float x, float y, float z) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform3f(idx, x, y, z);
    }
}

void Program::setVec4(const std::string &name, const glm::vec4 &value) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform4fv(idx, 1, glm::value_ptr(value));
    }
}

void Program::setVec4(const std::string &name, float x, float y, float z, float w) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniform4f(idx, x, y, z, w);
    }
}

void Program::setMat2(const std::string &name, const glm::mat2& mat) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniformMatrix2fv(idx, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Program::setMat3(const std::string &name, const glm::mat3 &mat) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniformMatrix3fv(idx, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Program::setMat4(const std::string &name, const glm::mat4 &mat) {
    GLint idx = glGetUniformLocation(ID_, name.c_str());
    if (idx < 0) {
        fmt::print("WARN::SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}\n", name);
    } else {
        glUniformMatrix4fv(idx, 1, GL_FALSE, glm::value_ptr(mat));
    }
}