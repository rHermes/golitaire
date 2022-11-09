//
// Created by rhermes on 10/9/22.
//

#pragma once

#include <memory>
#include <cstddef>

namespace gol {
    namespace utils {
        template <class T>
        inline void hash_combine(std::size_t& seed, const T& v)
        {
            std::hash<T> hasher;
            seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
    }

} // gol
