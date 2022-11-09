//
// Created by rhermes on 11/2/22.
//

#include <utility>
#include "Buffer.h"

using namespace LTK;

Buffer::Buffer(Buffer::Type type) : type_{type} {
    glGenBuffers(1, &ID_);
}

Buffer::Buffer(Buffer::Type type, GLuint ID) : type_{type}, ID_{ID} {
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &ID_);
}

GLuint Buffer::id() const {
    return ID_;
}

void Buffer::bind() {
    glBindBuffer(static_cast<GLenum>(type_), ID_);
}

Buffer::Buffer(Buffer &&other) noexcept {
    *this = std::move(other);
}

Buffer& Buffer::operator=(Buffer &&other) noexcept {
    ID_ = std::exchange(other.ID_, 0);
    type_ = other.type_;

    return *this;
}



