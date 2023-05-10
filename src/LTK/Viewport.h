//
// Created by rhermes on 5/10/23.
//

#pragma once

#include <cstdint>
#include <glad/gles2.h>


namespace LTK {

    /// Viewport is just a window. It's important to note that the bottom of the viewport is 0y.
    class Viewport {
    private:
        std::int32_t left_{0};
        std::int32_t bottom_{0};
        std::uint32_t width_{0};
        std::uint32_t height_{0};

    public:

        Viewport(std::int32_t left, std::int32_t bottom, std::uint32_t width, std::uint32_t height)
            : left_{left}
            , bottom_{bottom}
            , width_{width}
            , height_{height}
        {}

        Viewport(std::uint32_t width, std::uint32_t height) : Viewport(0, 0, width, height) {}


        [[nodiscard]] std::uint32_t width() const {
            return width_;
        }

        [[nodiscard]] std::uint32_t height() const {
            return height_;
        }

        [[nodiscard]] std::int32_t left() const {
            return left_;
        }

        [[nodiscard]] std::int32_t bottom() const {
            return bottom_;
        }

        [[nodiscard]] std::int32_t top() const {
            return bottom_ + static_cast<std::int32_t>(height_);
        }

        [[nodiscard]] std::int32_t right() const {
            return left_ + static_cast<std::int32_t>(width_);
        }

        void setWidth(const std::uint32_t width) {
            width_ = width;
        }

        void setHeight(const std::uint32_t height) {
            height_ = height;
        }

        void setLeft(const std::int32_t left) {
            left_ = left;
        }

        void setBottom(const std::int32_t bottom) {
            bottom_ = bottom;
        }

        /// Apply the viewport
        void apply() const {
            glViewport(left_, bottom_, static_cast<GLint>(width_), static_cast<GLint>(height_));
        }
    };

} // LTK
