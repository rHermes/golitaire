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
        std::map<Key, ShaderProgram> programs_{};
        std::map<Key, Texture2D> textures_{};

        Texture2D defaultTexture_;

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


    };

} // LTK
