//
// Created by rhermes on 9/10/22.
//

#pragma once

#include <glad/gles2.h>

#include <string_view>
#include "fmt/core.h"
#include "fmt/format.h"

namespace LTK {

    class Shader {
    public:
        enum class Type {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
        };

    private:
        GLuint ID_{0};

    public:
        explicit Shader(GLuint ID);

        Shader(Type type, const std::string_view& sourceCode);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        [[nodiscard]] GLuint id() const;

        // Load a shader from disk and compile it.
        static Shader loadFromDisk(Type type, const std::string_view& path);



    };
} // LTK


template <> struct fmt::formatter<LTK::Shader::Type>: fmt::formatter<string_view> {
    template <typename FormatContext>
    auto format(LTK::Shader::Type type, FormatContext& ctx) const {
        string_view name = "UNKNOWN";
        switch (type) {
            case LTK::Shader::Type::Fragment: name = "FRAGMENT"; break;
            case LTK::Shader::Type::Vertex: name = "VERTEX"; break;
        }
        return fmt::formatter<string_view>::format(name, ctx);
    }
};