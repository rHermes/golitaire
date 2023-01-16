//
// Created by rhermes on 10/9/22.
//

#pragma once

#include <memory>
#include <cstddef>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace gol {
    namespace utils {
        template <class T>
        inline void hash_combine(std::size_t& seed, const T& v)
        {
            std::hash<T> hasher;
            seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        [[nodiscard]] inline glm::vec3 toNDC(const glm::vec4& v) {
            return (glm::vec3(v.x, v.y, v.z) / v.w);
        }
    }
} // gol
