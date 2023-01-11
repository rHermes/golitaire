//
// Created by rhermes on 1/11/23.
//

#include <memory>
#include "Ray.h"
#include "Plane.h"
#include "glm/geometric.hpp"

using namespace LTK;

Ray::Ray(glm::vec3 origin, glm::vec3 direction) : origin_{std::move(origin)}, direction_{std::move(direction)} {
}

// Taken from:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection.html
std::optional<glm::vec3> Ray::intersect(const Plane &plane) const {
    const auto& n = plane.getNormal();
    const auto& p0 = plane.getOrigin();


    const auto denom = glm::dot(n, direction_);

    if (1e-6 < denom) {
        const auto p0l0 = p0 - origin_;
        const auto t = glm::dot(p0l0, n) / denom;
        if (0 <= t) {
            return origin_ + direction_*t;
        }
    }

    return std::nullopt;
}

std::optional<glm::vec3> Ray::intersect(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const {
    const auto edge0 = v1 - v0;
    const auto edge1 = v2 - v0;

    const auto normal = glm::cross(edge1, edge0);

    const LTK::Plane pl{v0, normal};

    const auto pHit = intersect(pl);
    if (!pHit) {
        return std::nullopt;
    }

    const auto edge2 = v1 - v2;

    const auto p0 = *pHit - v0;
    const auto p1 = *pHit - v1;
    const auto p2 = *pHit - v2;

    if (
            glm::dot(glm::cross(edge1, p0), normal) < 0
            || glm::dot(glm::cross(edge2, p2), normal) < 0
            || glm::dot(glm::cross(-edge1, p1), normal) < 0
            )
        return std::nullopt;

    return pHit;
}
