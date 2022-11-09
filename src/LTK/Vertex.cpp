//
// Created by rhermes on 11/2/22.
//

#include "Vertex.h"
#include "glad/gles2.h"

using namespace LTK;

void Vertex::setupAttribs() {
    glVertexAttribPointer(0, decltype(vertexPos)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, vertexPos)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, decltype(color)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, decltype(texCoord)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(2);
}