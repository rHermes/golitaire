//
// Created by rhermes on 9/28/22.
//

#pragma once

#include <glad/gles2.h>

namespace LTK {
    enum class ImageFormat {
        RGB = GL_RGB,
    };

    enum class ImageWrap {
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
}