//
// Created by rhermes on 9/11/22.
//

#include <utility>
#include "Texture2D.h"
#include "stb/stb_image.h"

using namespace LTK;

Texture2D::Texture2D(GLuint ID) : ID_{ID} {
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &ID_);
}

GLuint Texture2D::id() const {
    return ID_;
}

void Texture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, ID_);
}

Texture2D Texture2D::loadFromDisk(const std::filesystem::path &path, const bool preserveBind) {
    int width, height, bands;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &bands, 0);
    if (!data) {
        throw std::runtime_error("Couldn't load texture");
    }

    GLint format;
    switch (bands) {
        case 4:
            format = GL_RGBA;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 2:
            format = GL_LUMINANCE_ALPHA;
            break;
        case 1:
            format = GL_ALPHA;
            break;
        default:
            stbi_image_free(data);
            throw std::runtime_error("unknown number of bands");
    }

    GLuint ID;
    glGenTextures(1, &ID);

    // We record the old texture, so we can restore it after our loading.
    GLint prevID;
    if (preserveBind) {
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevID);
    }

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // Base mipmap level
                 format, // internal format
                 width,
                 height,
                 0, // Legacy parameter called border, must be 0
                 format, // Image format
                 GL_UNSIGNED_BYTE, // Pixel data format
                 data);

    // We no longer need the image data in memory, and we release it.
    stbi_image_free(data);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    if (preserveBind) {
        // Restore the old texture
        glBindTexture(GL_TEXTURE_2D, prevID);
    }

    return Texture2D(ID);
}

Texture2D::Texture2D(Texture2D &&other) noexcept {
    *this = std::move(other);
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
    ID_ = std::exchange(other.ID_, 0);
    return *this;
}

