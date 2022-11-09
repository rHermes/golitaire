//
// Created by rhermes on 11/2/22.
//

#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace LTK {

    struct alignas(16) Vertex {
        glm::vec3 vertexPos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static void setupAttribs();
    };

} // LTK
