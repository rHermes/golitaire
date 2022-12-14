//
// Created by rhermes on 10/9/22.
//

#include <vector>
#include "SceneRenderer.h"
#include "LTK/Vertex.h"
#include "LTK/Buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

using namespace gol;

SceneRenderer::~SceneRenderer() {
    resManager_->removeTexture(backgroundTexture_);
    resManager_->removeTexture(cardBack_);

    for (const auto& texKey : cardTextures_) {
        resManager_->removeTexture(texKey.second);
    }

    resManager_->removeProgram(cardShader_);
    resManager_->removeProgram(backgroundShader_);
}

SceneRenderer::SceneRenderer(std::shared_ptr<LTK::ResourceManager> resManager) : resManager_{std::move(resManager)} {

    backgroundShader_ = resManager_->loadShaderProgram("data/shaders/screen-space.vert", "data/shaders/default.frag");
    backgroundTexture_ = resManager_->loadTexture("data/textures/background.jpg", true);

    cardShader_ = resManager_->loadShaderProgram("data/shaders/default.vert", "data/shaders/default.frag");

    const std::filesystem::path base = "data/textures/cards/base";
    // We need to init things here
    std::map<std::string, CardSuite> nameToSuite = {
            {"club", CardSuite::Clubs},
            {"spade", CardSuite::Spades},
            {"diamond", CardSuite::Diamonds},
            {"heart", CardSuite::Hearts},
    };

    std::map<std::string, CardMember> nameToMember = {
            {"2", CardMember::Two},
            {"3", CardMember::Three},
            {"4", CardMember::Four},
            {"5", CardMember::Five},
            {"6", CardMember::Six},
            {"7", CardMember::Seven},
            {"8", CardMember::Eight},
            {"9", CardMember::Nine},
            {"10", CardMember::Ten},
            {"jack", CardMember::Jack},
            {"queen", CardMember::Queen},
            {"king", CardMember::King},
            {"1", CardMember::Ace},
    };

    cardBack_ = resManager_->loadTexture(base / "back.png", true);

    // Load jokers
    cardTextures_[{CardSuite::Joker, CardMember::BlackJoker}] = resManager_->loadTexture(base / "joker_black.png", true);
    cardTextures_[{CardSuite::Joker, CardMember::RedJoker}] = resManager_->loadTexture(base / "joker_red.png", true);

    // We load all the decks.
    for (const auto& [suiteName, suite] : nameToSuite) {
        for (const auto& [memberName, member] : nameToMember) {
            cardTextures_[{suite, member}] = resManager_->loadTexture(base / (suiteName + "_" + memberName + ".png"), true);
        }
    }

    const std::vector<LTK::Vertex> backgroundVertices = {
            // Front face
            {{-1.0f, 1.0f, 0.9999f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // Front face, Upper left
            {{ 1.0f, 1.0f, 0.9999f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Front face, Upper right
            {{ 1.0f,-1.0f, 0.9999f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // Front face, Lower right
            {{-1.0f,-1.0f, 0.9999f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // Front face, Lower left
    };

    const std::vector<glm::uvec3> backgroundIndices = {
            {0, 1, 2}, // First triangle
            {2, 3, 0}, // Second triangle
    };

    // We need to prepare the card unit mesh, as we are going to be drawing it.
    // It's in  upper left, upper right, lower right, lower left
    // We need 2 of them, as we need to have the texture coordinate the other way on the back of the card
    const std::vector<LTK::Vertex> cardFrontVertices = {
            // Front face
            {{-0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // Front face, Upper left
            {{ 0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Front face, Upper right
            {{ 0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // Front face, Lower right
            {{-0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // Front face, Lower left
    };

    const std::vector<glm::uvec3> cardFrontIndices = {
            {0, 1, 2}, // First triangle
            {2, 3, 0}, // Second triangle
    };

    const std::vector<LTK::Vertex> cardBackVertices = {
            // BackFace
            {{-0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Back Face Upper left
            {{ 0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // Back Face, Upper right
            {{ 0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // Back Face, Lower right
            {{-0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // Back Face, Lower left
    };
    /*
    const std::vector<LTK::Vertex> cardBackVertices = {
            // BackFace
            {{-cardWidth_/2,  cardHeight_/2, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Back Face Upper left
            {{ cardWidth_/2,  cardHeight_/2, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // Back Face, Upper right
            {{ cardWidth_/2, -cardHeight_/2, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // Back Face, Lower right
            {{-cardWidth_/2, -cardHeight_/2, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // Back Face, Lower left
    };
     */

    const std::vector<glm::uvec3> cardBackIndices = {
            {1, 0, 3}, // First triangle
            {3, 2, 1}, // Second triangle
    };

    // Background
    backgroundVAO_.bind();
    backgroundVBO_.bind();
    backgroundVBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(backgroundVertices));
    LTK::Vertex::setupAttribs();

    backgroundEBO_.bind();
    backgroundEBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(backgroundIndices));

    // Bind the front
    cardFrontVAO_.bind();

    cardFrontVBO_.bind();
    cardFrontVBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardFrontVertices));
    LTK::Vertex::setupAttribs();

    cardFrontEBO_.bind();
    cardFrontEBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardFrontIndices));

    // Bind the back
    cardBackVAO_.bind();

    cardBackVBO_.bind();
    cardBackVBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardBackVertices));
    LTK::Vertex::setupAttribs();

    cardBackEBO_.bind();
    cardBackEBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardBackIndices));

    cardBackVAO_.unbind();
}

void SceneRenderer::render(const int windowWidth, const int windowHeight, const float cardsWide, const float cardsTall) {
    const float fWW = static_cast<float>(windowWidth);
    const float fWH = static_cast<float>(windowHeight);

    const float boardBuffer = 15;

    const auto boardWidth = cardWidth_ * cardsWide + 2*boardBuffer;
    const auto boardHeight = cardHeight_ * cardsTall +2*boardBuffer;

    const auto boardScale = std::min(fWW/boardWidth, fWH/boardHeight);

    const auto boardWidthPx = boardScale * boardWidth;
    const auto boardHeightPx = boardScale * boardHeight;

    glm::mat4 cardWorld = glm::mat4(1.0f);

    // Now move it to the center of the screen
    cardWorld = glm::translate(cardWorld, {(fWW - boardWidthPx)/2, (fWH - boardHeightPx)/2, 0});

    // Apply the buffer
    cardWorld = glm::translate(cardWorld, {boardBuffer, boardBuffer, 0});
    cardWorld = glm::scale(cardWorld, {boardScale, boardScale, 0});
    cardWorld = glm::translate(cardWorld, {cardWidth_/2, cardHeight_/2, 0});
    cardWorld = glm::scale(cardWorld, glm::vec3(cardWidth_, cardHeight_,0));

    const glm::vec3 cameraPos{0.0f, 0.0f, 10.0f};
    glm::mat4 view = glm::mat4(1.0f);

    // Eye, center, up
    view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), 0.1f, 100.0f);

    const glm::mat4 vp = projection * view;

    // We draw the background
    {
        backgroundVAO_.bind();
        resManager_->getTexture(backgroundTexture_).bind();
        resManager_->getProgram(backgroundShader_).use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Draw each card.
    auto &prog = resManager_->getProgram(cardShader_);
    prog.use();

    // TODO(rHermes): Don't do this at runtime, but rather once the cards have been updated
    std::multimap<float, decltype(cards_)::value_type> cardsSorted;
    for (const auto& card : cards_) {
        cardsSorted.emplace(-card->getPosition().z, card);
    }

    // We now draw them in reverse order
    for (auto it = std::rbegin(cardsSorted); it != std::rend(cardsSorted); ++it) {
        const auto& card = it->second;

        // We move the things to the middle of the screen
        const glm::mat4 mvp = vp * cardWorld * card->getTransform();
        prog.setMat4("mvp", mvp);


        cardFrontVAO_.bind();
        resManager_->getTexture(cardTextures_[card->getType()]).bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        cardBackVAO_.bind();
        resManager_->getTexture(cardBack_).bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SceneRenderer::addCard(std::shared_ptr<Card> card) {
    cards_.insert(std::move(card));
}

void SceneRenderer::removeCard(const std::shared_ptr<Card> &card) {
    cards_.erase(card);
}
