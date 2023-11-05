//
// Created by rhermes on 1/18/23.
//

#include "EllipseRenderer2D.h"
#include "glad/gles2.h"


using namespace LTK;

void EllipseRenderer2D::render(const GraphicsRenderer2D& renderer, const glm::mat4& proj, const glm::mat4& view) {
    // We always need to check if we need to update the data.
    ellipseData_.updateData();


}

EllipseRenderer2D::EllipseRenderer2D() {
    vao_.bind();

    // Manually create quad. Remember it's clockwise
    unitVBO_.push_back({{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}});
    unitVBO_.push_back({{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}});
    unitVBO_.push_back({{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}});

    unitVBO_.push_back({{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}});
    unitVBO_.push_back({{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}});
    unitVBO_.push_back({{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}});

    unitVBO_.bind();
    UnitQuadData::setupAttribs();

    ellipseData_.bind();
    EllipseData::setupAttribs();

    unbindVAO();
}

void EllipseRenderer2D::bindVAO() const {
    vao_.bind();
}

std::size_t EllipseRenderer2D::count() const {
    return ellipses_.size();
}

void EllipseRenderer2D::updateEllipseData(const std::size_t index) {
    ellipseData_[index].transform = ellipses_[index]->getTransform();
    ellipseData_[index].color = ellipses_[index]->getColor();
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

void EllipseRenderer2D::UnitQuadData::setupAttribs() {
    const auto stride_size = sizeof(UnitQuadData);
    const auto position_offset = offsetof(UnitQuadData, position);
    const auto texture_coordinate_offset = offsetof(UnitQuadData, texture_coordinate);

    const auto texture_coordinate_location = 2;
    const auto position_location = 0;

    // Position location
    glVertexAttribPointer(position_location, decltype(position)::length(), GL_FLOAT, false, stride_size, reinterpret_cast<void*>(position_offset));
    glEnableVertexAttribArray(position_location);

    // Texture coordinates
    glVertexAttribPointer(texture_coordinate_location, decltype(texture_coordinate)::length(), GL_FLOAT, false, stride_size, reinterpret_cast<void*>(texture_coordinate_offset));
    glEnableVertexAttribArray(texture_coordinate_location);
}
