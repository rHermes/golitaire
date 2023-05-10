//
// Created by rhermes on 11/2/22.
//

#pragma once

#include <utility>
#include <span>
#include "glad/gles2.h"
#include "Misc.h"

namespace LTK {
    template <typename T>
    class Buffer {
    private:
        BufferType type_;
        BufferUsage usage_;
        std::size_t size_{0};

        GLuint ID_{0};


    public:


        Buffer(BufferType type, BufferUsage usage, std::size_t size) : type_{type}, usage_{usage}, size_{size} {
            glGenBuffers(1, &ID_);
            if (0 < size_) {
                glBindBuffer(GL_COPY_WRITE_BUFFER, ID_);
                glBufferData(GL_COPY_WRITE_BUFFER,
                             static_cast<GLsizeiptr>(size_ * sizeof(T)), nullptr, static_cast<GLenum>(usage_));
            }
        };

        Buffer() : type_{BufferType::Array}, usage_{BufferUsage::DynamicDraw} {
            glGenBuffers(1, &ID_);
        }

        Buffer(BufferType type, BufferUsage usage, std::size_t size, GLuint ID) : type_{type}, usage_{usage}, size_{size},  ID_{ID} {};

        Buffer(BufferType type, BufferUsage usage, std::span<T> data) : type_{type}, usage_{usage}, size_{data.size()} {
            glGenBuffers(1, &ID_);
            if (0 < size_) {
                glBindBuffer(GL_COPY_WRITE_BUFFER, ID_);
                glBufferData(GL_COPY_WRITE_BUFFER, data.size_bytes(),
                             reinterpret_cast<const GLvoid*>(data.data()), static_cast<GLenum>(usage_));
            }
        }

        ~Buffer() {
            glDeleteBuffers(1, &ID_);
        }

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        Buffer(Buffer&& other) noexcept : type_{other.type_}, usage_{other.usage_}, size_{other.size_} {
            *this = std::move(other);
        };

        Buffer& operator=(Buffer&& other) noexcept {
            ID_ = std::exchange(other.ID_, 0);
            type_ = other.type_;
            usage_ = other.usage_;
            size_ = other.size_;

            return *this;
        };

        [[nodiscard]] GLuint id() const {
            return ID_;
        };

        [[nodiscard]] std::size_t size() const {
            return size_;
        }

        [[nodiscard]] BufferUsage usage() const {
            return usage_;
        }

        [[nodiscard]] BufferType type() const {
            return type_;
        }

        void bind(const BufferType type) {
            glBindBuffer(static_cast<GLenum>(type), ID_);
        }

        void bind() {
            bind(type_);
        }

        // This will invalidate the old data
        void bufferData(BufferUsage usage, std::span<const T> data, bool do_bind = true) {
            auto tp = type_;
            if (do_bind) {
                tp = BufferType::CopyWrite;
                this->bind(tp);
            }

            glBufferData(static_cast<GLenum>(tp), data.size_bytes(),
                         reinterpret_cast<const GLvoid*>(data.data()), static_cast<GLenum>(usage));

            usage_ = usage;
            size_ = data.size();
        }

    };
} // LTK
