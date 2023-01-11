//
// Created by rhermes on 1/11/23.
//

#include "Plane.h"
#include "glm/geometric.hpp"
#include <memory>

using namespace LTK;

Plane::Plane(glm::vec3 origin, glm::vec3 normal) : origin_{std::move(origin)}, normal_{std::move(normal)}{
    normal_ = glm::normalize(normal_);
}