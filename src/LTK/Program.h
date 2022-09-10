//
// Created by rhermes on 9/10/22.
//

#pragma once

#include "Shader.h"

#include <glad/gles2.h>
#include <span>


namespace LTK {

    class Program {
    private:
        GLuint ID_{0};

    public:
        explicit Program(GLuint ID);
        ~Program();

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        explicit Program(std::span<std::reference_wrapper<const Shader>> shaders);
        Program(std::initializer_list<std::reference_wrapper<const Shader>> shaders);

        /// use/activate the program
        void use() const;

        /// Attempt to link the program
        void link();

        /// Attach a shader to the program
        void attach(const Shader& shader);

        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
    };

} // LTK
