//
// Created by rhermes on 10/9/22.
//

#pragma once

#include <unordered_map>
#include <set>
#include "LTK/ResourceManager.h"
#include "CardEnums.h"
#include "LTK/VAO.h"
#include "Card.h"
#include "LTK/Buffer.h"
#include "LTK/Vertex.h"

namespace gol {

    class SceneRenderer {
    private:
        using PKey = LTK::ResourceManager::Key;

        static constexpr float cardWidth_ = 63.0f;
        static constexpr float cardHeight_ = 88.0f;

        const std::vector<LTK::Vertex> cardFrontVertices_ = {
                // Front face
                {{-0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // Front face, Upper left
                {{ 0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Front face, Upper right
                {{ 0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // Front face, Lower right
                {{-0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // Front face, Lower left
        };

        const std::vector<glm::uvec3> cardFrontIndices_ = {
                {0, 1, 2}, // First triangle
                {2, 3, 0}, // Second triangle
        };

        int windowWidth_{1};
        int windowHeight_{1};

        float cardsTall_{0.1f};
        float cardsWide_{0.1f};

        std::shared_ptr<LTK::ResourceManager> resManager_;

        PKey backgroundShader_{0};
        PKey backgroundTexture_{0};


        LTK::Buffer backgroundVBO_{LTK::Buffer::Type::VBO};
        LTK::Buffer backgroundEBO_{LTK::Buffer::Type::EBO};
        LTK::VAO backgroundVAO_{};

        PKey cardShader_{0};

        PKey cardBack_{0};
        std::unordered_map<CardType, PKey> cardTextures_;

        LTK::Buffer cardFrontVBO_{LTK::Buffer::Type::VBO};
        LTK::Buffer cardFrontEBO_{LTK::Buffer::Type::EBO};
        LTK::VAO cardFrontVAO_{};

        LTK::Buffer cardBackVBO_{LTK::Buffer::Type::VBO};
        LTK::Buffer cardBackEBO_{LTK::Buffer::Type::EBO};
        LTK::VAO cardBackVAO_{};

        std::set<std::shared_ptr<Card>> cards_;

        glm::mat4 viewModel_;
        glm::mat4 projModel_;

        glm::mat4 viewport_;
        glm::mat4 cardWorld_;
        void recomputeMatrixes();

    public:
        explicit SceneRenderer(std::shared_ptr<LTK::ResourceManager> resManager);
        ~SceneRenderer();

        // A scene renderer cannot be copied
        SceneRenderer(const SceneRenderer&) = delete;
        SceneRenderer& operator=(const SceneRenderer&) = delete;

        // Haven't implemented this yet, so just deleting for now
        SceneRenderer(SceneRenderer&&) = delete;
        SceneRenderer& operator=(SceneRenderer&&) = delete;

        void addCard(std::shared_ptr<Card> card);
        void removeCard(const std::shared_ptr<Card>& card);

        void setCardsWide(float cardsWide);
        void setCardsTall(float cardsTall);
        void setWindowSize(int width, int height);

        // Returns the topmost card, if any, that intersects with the given point.
        [[nodiscard]] std::shared_ptr<Card> hitTestCards(const glm::vec2& pos) const;

        void render();
    };

} // gol
