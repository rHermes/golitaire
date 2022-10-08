//
// Created by rhermes on 9/10/22.
//

#pragma once

#include "Shader.h"
#include "glm/fwd.hpp"

#include <glad/gles2.h>
#include <span>


namespace LTK {

    class ShaderProgram {
    private:
        GLuint ID_{0};

    public:
        explicit ShaderProgram(GLuint ID);
        ~ShaderProgram();

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;


        ShaderProgram(ShaderProgram&& other) noexcept;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        explicit ShaderProgram(std::span<std::reference_wrapper<const Shader>> shaders);
        ShaderProgram(std::initializer_list<std::reference_wrapper<const Shader>> shaders);

        /// use/activate the program
        void use() const;

        /// Attempt to link the program
        void link();

        /// Attach a shader to the program
        void attach(const Shader& shader);

        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);

        void setVec2(const std::string& name, const glm::vec2& value);
        void setVec2(const std::string& name, float x, float y);

        void setVec3(const std::string& name, const glm::vec3& value);
        void setVec3(const std::string& name, float x, float y, float z);

        void setVec4(const std::string& name, const glm::vec4& value);
        void setVec4(const std::string& name, float x, float y, float z, float w);

        void setMat2(const std::string& name, const glm::mat2& mat);
        void setMat3(const std::string& name, const glm::mat3& mat);
        void setMat4(const std::string& name, const glm::mat4& mat);
    };

} // LTK
