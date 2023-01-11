//
// Created by rhermes on 1/11/23.
//

#pragma once


#include <optional>
#include "glm/vec3.hpp"

namespace LTK {
    class Plane;

    class Ray {
    private:
        glm::vec3 origin_{};
        glm::vec3 direction_{};

    public:
        Ray(glm::vec3 origin, glm::vec3 direction);

        [[nodiscard]] std::optional<glm::vec3> intersect(const LTK::Plane& plane) const;

        /// Intersection with triangle, given in clockwise direction
        [[nodiscard]] std::optional<glm::vec3> intersect(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
    };

} // LTK
