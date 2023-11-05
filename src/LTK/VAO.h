//
// Created by rhermes on 11/2/22.
//

#pragma once

#include "glad/gles2.h"

namespace LTK {

    class VAO {

    private:
        GLuint ID_{0};

    public:
        VAO();
        explicit VAO(GLuint ID);

        ~VAO();

        VAO(const VAO&) = delete;
        VAO& operator=(const VAO&) = delete;

        VAO(VAO&& other) noexcept;
        VAO& operator=(VAO&& other) noexcept;

        [[nodiscard]] GLuint id() const;

        void bind() const;
    };
} // LTK
