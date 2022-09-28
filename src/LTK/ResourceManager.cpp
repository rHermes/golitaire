//
// Created by rhermes on 9/28/22.
//

#include "ResourceManager.h"

using namespace LTK;





ShaderProgram &ResourceManager::getProgram(const ResourceManager::Key &id) {
    const auto it = programs_.find(id);
    if (it == programs_.end())
        throw std::runtime_error("Couldn't find program");

    return it->second;
}

const ShaderProgram &ResourceManager::getProgram(const ResourceManager::Key &id) const {
    const auto it = programs_.find(id);
    if (it == programs_.end())
        throw std::runtime_error("Couldn't find program");

    return it->second;
}

Texture2D &ResourceManager::getTexture(const ResourceManager::Key &id) {
    const auto it = textures_.find(id);
    if (it == textures_.end())
        throw std::runtime_error("Couldn't find program");

    return it->second;
}

const Texture2D &ResourceManager::getTexture(const ResourceManager::Key &id) const {
    return <#initializer#>;
}