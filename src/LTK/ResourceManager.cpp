//
// Created by rhermes on 9/28/22.
//

#include "ResourceManager.h"

#include "spdlog/spdlog.h"

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
    if (it == textures_.end()) {
        spdlog::warn("Couldn't find texture, returning default!");
        return defaultTexture_;
    }

    return it->second;
}

const Texture2D &ResourceManager::getTexture(const ResourceManager::Key &id) const {
    const auto it = textures_.find(id);
    if (it == textures_.end()) {
        spdlog::warn("Couldn't find texture, returning default!");
        return defaultTexture_;
    }

    return it->second;
}

ResourceManager::ResourceManager() {

    // We generate a 101x101 test texture.
    constexpr size_t width = 101;
    constexpr size_t height = 101;

    std::vector<GLubyte> testData(3 * width * height, 0);

    for (decltype(testData)::size_type y = 0; y < height; y++) {
        for (decltype(testData)::size_type x = 0; x < width; x++) {
            // Yellow default
            std::array<GLubyte, 3> color = {255, 255, 0};
            const bool lowerHalf = y <= (height/2);
            const bool leftSide = x <= (width/2);


            if (lowerHalf) {
                if (leftSide) {
                    color = {128, 255, 0}; // Green
                } else {
                    color = {255, 102, 102}; // Light red
                }
            } else {
                if (leftSide) {
                    color = {255, 255, 0}; // Yellow
                } else {
                    color = {51, 51, 255}; // Blue
                }
            }

            if (y == (height/2) || x == (width/2)) {
                color = {0, 0, 0}; // Black
            }
            const auto idx = 3* (y * width + x);
            testData[idx] = color[0];
            testData[idx+1] = color[1];
            testData[idx+2] = color[2];
        }
    }

    defaultTexture_.upload(width, height, testData);
}
