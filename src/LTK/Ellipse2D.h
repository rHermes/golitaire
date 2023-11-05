//
// Created by rhermes on 1/18/23.
//

#pragma once

// #include "EllipseRenderer2D.h"

#include <cstddef>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "Item3D.h"

namespace LTK {

    class EllipseRenderer2D;

    class Ellipse2D final : public Item3D {
    private:
        EllipseRenderer2D& renderer_;
        std::size_t renderIndex_;

        glm::vec4 color_;


        void updateTransform() const override;

    public:
        explicit Ellipse2D(EllipseRenderer2D& renderer, size_t renderIndex);

        glm::vec4 getColor() {
            return color_;
        }

        const glm::vec4& getColor() const {
            return color_;
        }
    };

} // LTK
