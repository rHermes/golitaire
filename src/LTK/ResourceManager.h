//
// Created by rhermes on 9/28/22.
//

#pragma once

#include <string>
#include <map>
#include "Shader.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

namespace LTK {

    class ResourceManager {
    public:
        using Key = std::uint32_t;

    private:
        Key nextProgramKey_{1};
        std::map<Key, ShaderProgram> programs_{};

        Key nextTextureKey_{1};
        std::map<Key, Texture2D> textures_{};

        Texture2D defaultTexture_;


        // Return an unused shader key
        [[nodiscard]] Key getProgramKey();

        // Return an unused texture key
        [[nodiscard]] Key getTextureKey();

    public:
        ResourceManager();

        // A ResourceManager cannot be copied
        ResourceManager(const ResourceManager&) =delete;
        ResourceManager& operator=(const ResourceManager&) =delete;

        // Exception on not found.
        [[nodiscard]] ShaderProgram& getProgram(const Key& id);
        [[nodiscard]] const ShaderProgram& getProgram(const Key& id) const;

        // Returns default texture on failure.
        [[nodiscard]] Texture2D& getTexture(const Key& id);
        [[nodiscard]] const Texture2D& getTexture(const Key& id) const;

        void removeTexture(const Key& id);
        void removeProgram(const Key& id);

        [[nodiscard]] Key loadTexture(const std::filesystem::path& file, bool alpha);

        [[nodiscard]] Key loadShaderProgram(const std::filesystem::path& vertexFile, const std::filesystem::path& fragFile);
    };

} // LTK
