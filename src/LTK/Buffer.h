//
// Created by rhermes on 11/2/22.
//

#pragma once

#include <span>
#include "glad/gles2.h"

namespace LTK {

    enum class BufferUsage {
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY,
    };

    class Buffer {
    public:
        enum class Type : GLenum {
            VBO = GL_ARRAY_BUFFER,
            EBO = GL_ELEMENT_ARRAY_BUFFER,
        };

    private:
        Type type_;
        GLuint ID_{0};

    public:
        explicit Buffer(Type type);
        Buffer(Type type, GLuint ID);

        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        [[nodiscard]] GLuint id() const;

        void bind();

        template <typename T>
        void bufferData(BufferUsage usage, std::span<T> data) {
            glBufferData(static_cast<GLenum>(type_), data.size_bytes(),
                         reinterpret_cast<const GLvoid*>(data.data()), static_cast<GLenum>(usage));
        }

    };
} // LTK
