//
// Created by rhermes on 11/2/22.
//

#include <utility>
#include <iostream>
#include "VAO.h"

using namespace LTK;

VAO::VAO() {
    glGenVertexArrays(1, &ID_);
}

VAO::VAO(GLuint ID) : ID_{ID} {}

VAO::~VAO() {
    glDeleteVertexArrays(1, &ID_);
}

GLuint VAO::id() const {
    return ID_;
}

void VAO::bind() const {
    glBindVertexArray(ID_);
}

VAO::VAO(VAO &&other) noexcept {
    *this = std::move(other);
}

VAO& VAO::operator=(VAO &&other) noexcept {
    ID_ = std::exchange(other.ID_, 0);
    return *this;
}