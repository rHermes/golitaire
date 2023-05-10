//
// Created by rhermes on 11/2/22.
//

#pragma once

#include <utility>
#include <span>
#include "glad/gles2.h"
#include <vector>
#include "Misc.h"
#include "Buffer.h"
#include <spdlog/spdlog.h>

namespace LTK {
    /// A flexibuffer is an attempt to recreate std::vector.
    template <typename T>
    class FlexiBuffer {
    private:
        std::vector<T> vec_;
        Buffer<T> buffer_;

        bool dirty_{false};



    public:
        using value_type = T;

        FlexiBuffer(BufferType type, BufferUsage usage) : buffer_{type, usage, 0} {}

        FlexiBuffer(BufferType type, BufferUsage usage, std::span<const T> data) : vec_{data}, buffer_{type, usage, data} {}

        ~FlexiBuffer() = default;

        FlexiBuffer(const FlexiBuffer&) = delete;
        FlexiBuffer& operator=(const FlexiBuffer&) = delete;

        FlexiBuffer(FlexiBuffer&& other) noexcept {
            *this = std::move(other);
        };

        FlexiBuffer& operator=(FlexiBuffer&& other) noexcept {
            vec_ = std::move(other.vec_);
            buffer_ = std::move(other.buffer_);

            return *this;
        };

        [[nodiscard]] GLuint id() const {
            return buffer_.id();
        }

        [[nodiscard]] std::size_t size() const {
            return vec_.size();
        }

        void updateData() {
            if (!dirty_) {
                return;
            }

            buffer_.bufferData(buffer_.usage(), vec_, true);
            dirty_ = false;
        }

        void bind(const BufferType type) {
            // TODO(rHermes): This can be avoided, as we are binding it twice here. We could bind it directly,
            // but I'll take that later.
            if (dirty_)
                updateData();

            buffer_.bind(type);
        }

        void bind() {
            if (dirty_)
                updateData();

            buffer_.bind();
        }

        template< class... Args >
        auto emplace_back( Args&&... args ) {
            auto lel =  vec_.emplace_back(std::forward<Args>(args)...);
            dirty_ = true;

            return lel;
        }





    };
} // LTK
