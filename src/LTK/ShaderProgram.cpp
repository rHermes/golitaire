//
// Created by rhermes on 9/10/22.
//

#include <vector>
#include "ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"
#include <spdlog/spdlog.h>

using namespace LTK;

ShaderProgram::ShaderProgram(GLuint ID) : ID_{ID} {
}

void ShaderProgram::use() const {
    glUseProgram(ID_);
}

ShaderProgram::ShaderProgram(std::span<std::reference_wrapper<const Shader>> shaders) {
    ID_ = glCreateProgram();

    for (const auto& shader : shaders) {
        attach(shader);
    }

    link();
}

ShaderProgram::ShaderProgram(std::initializer_list<std::reference_wrapper<const Shader>> shaders) {
    ID_ = glCreateProgram();

    for (const auto& shader : shaders) {
        attach(shader);
    }

    link();
}


void ShaderProgram::link() {
    glLinkProgram(ID_);

    GLint success;
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        GLint bufferSize;
        glGetProgramiv(ID_, GL_INFO_LOG_LENGTH, &bufferSize);

        std::vector<GLchar> buffer(bufferSize + 1);
        glGetProgramInfoLog(ID_, static_cast<GLsizei>(buffer.size()), nullptr, buffer.data());

        spdlog::error("SHADER::PROGRAM::LINKING_FAILED:\n{}", buffer.data());
        throw std::runtime_error("Shader ShaderProgram linking failed");
    }
    // Only on new linking will the ID names potentially change, so we must clear it here.
    uniNameToId_.clear();
}

void ShaderProgram::attach(const Shader &shader) {
    glAttachShader(ID_, shader.id());
}

ShaderProgram::~ShaderProgram() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
    }
}

void ShaderProgram::setBool(const std::string &name, bool value) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform1i(idx, value);
    }
}

void ShaderProgram::setInt(const std::string &name, int value) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform1i(idx, value);
    }
}

void ShaderProgram::setFloat(const std::string &name, float value) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform1f(idx, value);
    }
}

void ShaderProgram::setVec2(const std::string &name, const glm::vec2 &value) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform2fv(idx, 1, glm::value_ptr(value));
    }
}

void ShaderProgram::setVec2(const std::string &name, float x, float y) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform2f(idx, x, y);
    }
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &value) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform3fv(idx, 1, glm::value_ptr(value));
    }
}

void ShaderProgram::setVec3(const std::string &name, float x, float y, float z) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform3f(idx, x, y, z);
    }
}

void ShaderProgram::setVec4(const std::string &name, const glm::vec4 &value) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform4fv(idx, 1, glm::value_ptr(value));
    }
}

void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniform4f(idx, x, y, z, w);
    }
}

void ShaderProgram::setMat2(const std::string &name, const glm::mat2& mat) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniformMatrix2fv(idx, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void ShaderProgram::setMat3(const std::string &name, const glm::mat3 &mat) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniformMatrix3fv(idx, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) {
    GLint idx = getUniformLocation(name);
    if (idx < 0) {
        spdlog::warn("SHADER::PROGRAM::UNKNOWN_UNIFORM_LOCATION: couldn't get uniform {}", name);
    } else {
        glUniformMatrix4fv(idx, 1, GL_FALSE, glm::value_ptr(mat));
    }
}


ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept {
    *this = std::move(other);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram &&other) noexcept {
    ID_ = std::exchange(other.ID_, 0);
    uniNameToId_ = std::move(other.uniNameToId_);
    return *this;
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const {
    if (const auto it = uniNameToId_.find(name); it != std::cend(uniNameToId_)) {
        return it->second;
    }

    GLint idx = glGetUniformLocation(ID_, name.c_str());
    // We cache all answers, since we don't have to waste time looking for negatives in the
    // future then.
    uniNameToId_.emplace(name, idx);

    return idx;
}
