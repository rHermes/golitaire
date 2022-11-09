//
// Created by rhermes on 9/11/22.
//

#pragma once

#include <filesystem>
#include <span>
#include "glad/gles2.h"
#include "Misc.h"

namespace LTK {

    /// Represents a Texture2D
    class Texture2D {
    private:
        GLuint ID_{0};

        // texture image dimensions
        GLsizei width_{0};
        GLsizei height_{0};

        // Texture format
        ImageFormat format_{ImageFormat::RGB};
        GLint internalFormat_{GL_RGB};

        // wrap operations
        ImageWrap wrapS_{ImageWrap::Repeat};
        ImageWrap wrapT_{ImageWrap::Repeat};
        TexMinFilter filterMin_{TexMinFilter::Linear};
        TexMagFilter filterMag_{TexMagFilter::Linear};


    public:

        Texture2D();

        explicit Texture2D(GLuint ID);
        ~Texture2D();

        // A texture cannot be duplicated
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        Texture2D(Texture2D&& other) noexcept;
        Texture2D& operator=(Texture2D&& other) noexcept;

        [[nodiscard]] GLuint id() const;

        // Upload the given data.
        void upload(GLsizei width, GLsizei height, std::span<GLubyte> data, bool preserveBind = true);

        // Bind texture
        void bind();

        void setFormat(ImageFormat format);
        void setInternalFormat(GLint internalFormat);


    };

} // LTK
