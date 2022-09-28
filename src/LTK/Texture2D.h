//
// Created by rhermes on 9/11/22.
//

#pragma once

#include <filesystem>
#include "glad/gles2.h"

namespace LTK {

    /// Represents a Texture2D
    class Texture2D {
    private:
        GLuint ID_{0};

    public:
        explicit Texture2D(GLuint ID);
        ~Texture2D();

        // A texture cannot be duplicated
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

        Texture2D(Texture2D&& other) noexcept;
        Texture2D& operator=(Texture2D&& other) noexcept;

        [[nodiscard]] GLuint id() const;

        // preserveBind indicates if we should restore the currently
        // bound texture, or if we can clobber it.
        static Texture2D loadFromDisk(const std::filesystem::path& path, bool preserveBind = true);

        void bind();
    };

} // LTK
