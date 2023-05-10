//
// Created by rhermes on 1/18/23.
//

#pragma once

#include "Buffer.h"
#include "FlexiBuffer.h"
#include "VAO.h"
#include "glm/detail/type_vec2.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "RenderItem2D.h"

#include <glm/fwd.hpp>

namespace LTK {

    class GraphicsRenderer2D;

    // A renderer for ellipses.
    class EllipseRenderer2D final : RenderItem2D {
    private:
        struct alignas(16) EllipseData {
            glm::mat4 transform; // Transform vertex
            glm::vec4 color; // Color

            static void setupAttribs();
        };

        VAO vao_;
        FlexiBuffer<glm::vec3> unitVBO_{BufferType::Array, BufferUsage::StaticDraw};
        FlexiBuffer<EllipseData> ellipseData_{BufferType::Array, BufferUsage::DynamicDraw};

    public:

        EllipseRenderer2D();
        ~EllipseRenderer2D() override = default;

        void render(const GraphicsRenderer2D& renderer) override;
    };

} // LTK
