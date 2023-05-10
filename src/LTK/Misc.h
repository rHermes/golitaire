//
// Created by rhermes on 9/28/22.
//

#pragma once

#include <glad/gles2.h>

namespace LTK {
    enum class ImageFormat : GLenum {
        RGB = GL_RGB,
        RGBA = GL_RGBA,
    };

    enum class ImageWrap : GLenum {
        Repeat = GL_REPEAT,
        MirrorRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
    };

    enum class TexMinFilter : GLint {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
    };

    enum class TexMagFilter : GLint {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
    };

    enum class BufferType : GLenum {
        Uniform = GL_UNIFORM_BUFFER,
        Array = GL_ARRAY_BUFFER,
        CopyRead = GL_COPY_READ_BUFFER,
        CopyWrite = GL_COPY_WRITE_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    };

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

    inline void unbindBufferType(const BufferType type) {
        glBindBuffer(static_cast<GLenum>(type), 0);
    }

    inline void unbindVAO() {
        glBindVertexArray(0);
    }
}