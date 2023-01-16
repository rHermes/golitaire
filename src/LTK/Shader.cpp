//
// Created by rhermes on 9/10/22.
//

#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include <fmt/core.h>
#include <vector>
#include <utility>
#include <spdlog/spdlog.h>
using namespace LTK;


Shader::Shader(GLuint ID) : ID_{ID} {}

Shader::Shader(Shader::Type type, const std::string_view &sourceCode) {
    ID_ = glCreateShader(static_cast<GLenum>(type));

    auto len = static_cast<GLint>(sourceCode.size());
    auto data = sourceCode.data();
    glShaderSource(ID_, 1, &data, &len);
    glCompileShader(ID_);

    GLint success;
    glGetShaderiv(ID_, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint bufferSize;
        glGetShaderiv(ID_, GL_INFO_LOG_LENGTH, &bufferSize);

        std::vector<GLchar> buffer(bufferSize + 1);
        glGetShaderInfoLog(ID_, static_cast<GLsizei>(buffer.size()), nullptr, buffer.data());

        spdlog::error("SHADER::{}::COMPILATION_FAILED:\n{}", type, buffer.data());

        throw std::runtime_error("Shader compilation failed");
    }
}

Shader Shader::loadFromDisk(Shader::Type type, const std::string_view &path) {
    std::string shaderCode;

    try {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(std::string(path));
        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
        shaderCode = shaderStream.str();

        return {type, shaderCode};
    } catch (std::ifstream::failure &e) {
        spdlog::error("SHADER::FILE_NOT_SUCCESSFULLY_READ: {}\n{}", path, e.what());
        throw;
    }
}

Shader::~Shader() {
    if (ID_ != 0) {
        glDeleteShader(ID_);
    }
}

GLuint Shader::id() const {
    return ID_;
}

Shader::Shader(Shader &&other) noexcept {
    *this = std::move(other);
}

Shader& Shader::operator=(Shader &&other) noexcept {
    ID_ = std::exchange(other.ID_, 0);
    return *this;
}
