//
// Created by rhermes on 1/11/23.
//

#pragma once

#include "glm/vec3.hpp"

namespace LTK {

    class Plane {
    private:
        glm::vec3 origin_;
        glm::vec3 normal_;

    public:
        Plane(glm::vec3 origin, glm::vec3 normal);

        [[nodiscard]] const glm::vec3& getOrigin() const { return origin_; };
        [[nodiscard]] const glm::vec3& getNormal() const { return normal_; };
    };

} // LTK
