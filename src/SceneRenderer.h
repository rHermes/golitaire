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

namespace gol {

    class SceneRenderer {
    private:
        using PKey = LTK::ResourceManager::Key;

        static constexpr float cardWidth_ = 64.0f;
        static constexpr float cardHeight_ = 89.0f;


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

        void render(int windowWidth, int windowHeight, float cardsWide, float cardsTall);
    };

} // gol
