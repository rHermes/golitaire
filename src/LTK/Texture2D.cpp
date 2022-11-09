//
// Created by rhermes on 9/11/22.
//

#include <utility>
#include "Texture2D.h"
#include "stb/stb_image.h"

using namespace LTK;

Texture2D::Texture2D() {
    glGenTextures(1, &ID_);
}

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

Texture2D::Texture2D(Texture2D &&other) noexcept {
    *this = std::move(other);
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
    ID_ = std::exchange(other.ID_, 0);
    return *this;
}

void Texture2D::upload(const GLsizei width,
                       const GLsizei height, std::span<GLubyte> data, const bool preserveBind) {
    width_ = width;
    height_ = height;

    // We record the old texture, so we can restore it after our loading.
    GLint prevID;
    if (preserveBind) {
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevID);
    }

    // Set the unpack alignment
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glBindTexture(GL_TEXTURE_2D, ID_);
    glTexImage2D(GL_TEXTURE_2D,
                 0, internalFormat_, width_, height_, 0,
                 static_cast<GLenum>(format_), GL_UNSIGNED_BYTE, data.data());

    // We set the texture modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filterMin_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filterMag_));

    // Generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    if (preserveBind) {
        // Restore the old texture
        glBindTexture(GL_TEXTURE_2D, prevID);
    }
}

void Texture2D::setFormat(const ImageFormat format) {
    format_ = format;
}

void Texture2D::setInternalFormat(const GLint internalFormat) {
    internalFormat_ = internalFormat;
}



