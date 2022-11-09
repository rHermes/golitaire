//
// Created by rhermes on 9/28/22.
//

#include "ResourceManager.h"

#include "spdlog/spdlog.h"
#include "stb/stb_image.h"
#include <array>

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

ResourceManager::Key ResourceManager::loadTexture(const std::filesystem::path &file, bool alpha) {

    Texture2D texture;

    int wanted_bands = 3;
    if (alpha) {
        texture.setFormat(ImageFormat::RGBA);
        texture.setInternalFormat(GL_RGBA);
        wanted_bands = 4;
    }

    stbi_set_flip_vertically_on_load(true);
    int width, height, bands;
    unsigned char* data = stbi_load(file.string().c_str(), &width, &height, &bands, wanted_bands);
    if (!data) {
        throw std::runtime_error("Couldn't load texture");
    }

    std::span dataSpan(data, width * height * bands);
    texture.upload(width, height, dataSpan);
    stbi_image_free(data);

    glObjectLabelKHR(GL_TEXTURE, texture.id(), file.string().size(), file.string().c_str());

    const Key newKey = getTextureKey();
    textures_.insert_or_assign(newKey, std::move(texture));


    return newKey;
}

ResourceManager::Key
ResourceManager::loadShaderProgram(const std::filesystem::path &vertexFile, const std::filesystem::path &fragFile) {
    Shader vshader = Shader::loadFromDisk(Shader::Type::Vertex, vertexFile.string().c_str());
    Shader fshader = Shader::loadFromDisk(Shader::Type::Fragment, fragFile.string().c_str());

    ShaderProgram prog{std::cref(vshader), std::cref(fshader)};

    const Key newKey = getProgramKey();
    programs_.insert_or_assign(newKey, std::move(prog));

    return newKey;
}

ResourceManager::Key ResourceManager::getProgramKey() {
    return nextProgramKey_++;
}

ResourceManager::Key ResourceManager::getTextureKey() {
    return nextTextureKey_++;
}

void ResourceManager::removeTexture(const ResourceManager::Key &id) {
    textures_.erase(id);
}

void ResourceManager::removeProgram(const ResourceManager::Key &id) {
    programs_.erase(id);
}
