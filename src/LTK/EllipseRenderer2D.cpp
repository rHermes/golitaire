//
// Created by rhermes on 1/18/23.
//

#include "EllipseRenderer2D.h"
#include "glad/gles2.h"


using namespace LTK;

void EllipseRenderer2D::render(const GraphicsRenderer2D& renderer) {
    // We always need to check if we need to update the data.
    ellipseData_.updateData();
}

EllipseRenderer2D::EllipseRenderer2D() {
    vao_.bind();

    // Manually create quad. Remember it's clockwise
    unitVBO_.emplace_back(-0.5f, 0.5f, 0.0f);
    unitVBO_.emplace_back(0.5f, 0.5f, 0.0f);
    unitVBO_.emplace_back(0.5f, -0.5f, 0.0f);

    unitVBO_.emplace_back(-0.5f, 0.5f, 0.0f);
    unitVBO_.emplace_back(0.5f, -0.5f, 0.0f);
    unitVBO_.emplace_back(-0.5f, -0.5f, 0.0f);

    unitVBO_.bind();
    using vboType = decltype(unitVBO_)::value_type;
    const auto stride_size = sizeof(vboType);
    const auto element_length = vboType::length();
    glVertexAttribPointer(0,
                          element_length, GL_FLOAT, false, stride_size, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    ellipseData_.bind();
    EllipseData::setupAttribs();

    unbindVAO();
}

void EllipseRenderer2D::EllipseData::setupAttribs() {
    const auto vec4size = sizeof(glm::vec4);
    const auto stride_size = sizeof(EllipseData);
    const auto transform_offset = offsetof(EllipseData, transform);
    const auto color_offset = offsetof(EllipseData, color);

    const auto transform_loc = 6;
    const auto color_loc = 1;

    glVertexAttribPointer(color_loc, decltype(color)::length(), GL_FLOAT, false, stride_size, reinterpret_cast<void*>(color_offset));
    glEnableVertexAttribArray(color_loc);
    glVertexAttribDivisor(color_loc, 1);

    // We need 4 attributes to store this, as that's the max vertex attribute really.
    for (std::uint32_t i = 0; i < 4; i++) {
        glVertexAttribPointer(transform_loc+i, 4, GL_FLOAT, false, stride_size, reinterpret_cast<void*>(transform_offset+i*vec4size));
        glEnableVertexAttribArray(transform_loc+i);
        glVertexAttribDivisor(transform_loc+i, 1);
    }
}
