//
// Created by rhermes on 10/9/22.
//

#include <vector>
#include "SceneRenderer.h"
#include "LTK/Vertex.h"
#include "LTK/Buffer.h"
#include "LTK/Ray.h"
#include "spdlog/spdlog.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"

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

    const std::filesystem::path base = "data/textures/cards/vector-cards";
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

    const std::vector<LTK::Vertex> cardBackVertices = {
            // BackFace
            {{-0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Back Face Upper left
            {{ 0.5,  0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // Back Face, Upper right
            {{ 0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // Back Face, Lower right
            {{-0.5, -0.5, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // Back Face, Lower left
    };

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
    cardFrontVBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardFrontVertices_));
    LTK::Vertex::setupAttribs();

    cardFrontEBO_.bind();
    cardFrontEBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardFrontIndices_));

    // Bind the back
    cardBackVAO_.bind();

    cardBackVBO_.bind();
    cardBackVBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardBackVertices));
    LTK::Vertex::setupAttribs();

    cardBackEBO_.bind();
    cardBackEBO_.bufferData(LTK::BufferUsage::StaticDraw, std::span(cardBackIndices));

    // We unbind the VAO, as we don't want it to be polluted in another place.
    LTK::unbindVAO();

    recomputeMatrixes();
}

void SceneRenderer::render() {
    // We draw the background
    {
        backgroundVAO_.bind();
        resManager_->getTexture(backgroundTexture_).bind();
        resManager_->getProgram(backgroundShader_).use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }



    const auto gg = viewport_ * cardWorld_;
    std::multimap<float, decltype(cards_)::value_type> cardsSorted;
    for (const auto& card : cards_) {
        const glm::mat4 mvp = gg * card->getTransform();
        const glm::vec3 lol = gol::utils::toNDC(mvp * glm::vec4(0, 0, 0, 1));

        cardsSorted.emplace(lol.z, card);
    }

    // Draw each card.
    auto &prog = resManager_->getProgram(cardShader_);
    prog.use();

    // We now draw them in reverse order
    for (auto it = std::rbegin(cardsSorted); it != std::rend(cardsSorted); ++it) {
        const auto& card = it->second;

        // We move the things to the middle of the screen
        const glm::mat4 mvp = viewport_ * cardWorld_ * card->getTransform();
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

void SceneRenderer::setCardsWide(float cardsWide) {
    cardsWide_ = cardsWide;
    recomputeMatrixes();
}

void SceneRenderer::setCardsTall(float cardsTall) {
    cardsTall_ = cardsTall;
    recomputeMatrixes();
}

void SceneRenderer::setWindowSize(int width, int height) {
    windowWidth_ = width;
    windowHeight_ = height;
    recomputeMatrixes();
}

void SceneRenderer::recomputeMatrixes() {
    const float fWW = static_cast<float>(windowWidth_);
    const float fWH = static_cast<float>(windowHeight_);

    const float boardBuffer = 15;

    const auto boardWidth = cardWidth_ * cardsWide_ + 2*boardBuffer;
    const auto boardHeight = cardHeight_ * cardsTall_ +2*boardBuffer;

    const auto boardScale = std::min(fWW/boardWidth, fWH/boardHeight);

    const auto boardWidthPx = boardScale * boardWidth;
    const auto boardHeightPx = boardScale * boardHeight;

    cardWorld_ = glm::mat4(1.0f);

    // Now move it to the center of the screen
    cardWorld_ = glm::translate(cardWorld_, {(fWW - boardWidthPx)/2, (fWH - boardHeightPx)/2, 0});

    // Apply the buffer
    cardWorld_ = glm::translate(cardWorld_, {boardBuffer, boardBuffer, 0});
    cardWorld_ = glm::scale(cardWorld_, {boardScale, boardScale, 1});
    cardWorld_ = glm::translate(cardWorld_, {cardWidth_/2, cardHeight_/2, 0});
    cardWorld_ = glm::scale(cardWorld_, glm::vec3(cardWidth_, cardHeight_, 1));

    const glm::vec3 cameraPos{0.0f, 0.0f, 70.0f};
    glm::mat4 view = glm::mat4(1.0f);

    // Eye, center, up
    view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::ortho(0.0f, static_cast<float>(windowWidth_), 0.0f, static_cast<float>(windowHeight_), 0.1f, 100.0f);

    viewModel_ = view;
    projModel_ = projection;
    viewport_ = projection * view;
}

std::shared_ptr<Card> SceneRenderer::hitTestCards(const glm::vec2 &pos) const {
    const glm::vec3 pOrig = gol::utils::toNDC(viewport_ * glm::vec4(pos, 0.1, 1));
    const glm::vec3 pEnd = gol::utils::toNDC(viewport_ * glm::vec4(pos, 100, 1));

    const glm::vec3 pDelta = pEnd - pOrig;
    const LTK::Ray ray(pOrig, pDelta);

    std::shared_ptr<Card> nearest{nullptr};
    auto minDist = std::numeric_limits<float>::max();

    for (const auto& card : cards_) {
        // We move the things to the middle of the screen
        const glm::mat4 mvp = viewport_ * cardWorld_ * card->getTransform();

        for (const auto& indices : cardFrontIndices_) {
            const glm::vec3 v0 = gol::utils::toNDC(mvp * glm::vec4(cardFrontVertices_[indices.x].vertexPos, 1));
            const glm::vec3 v1 = gol::utils::toNDC(mvp * glm::vec4(cardFrontVertices_[indices.y].vertexPos, 1));
            const glm::vec3 v2 = gol::utils::toNDC(mvp * glm::vec4(cardFrontVertices_[indices.z].vertexPos, 1));

            const auto pHit = ray.intersect(v0, v1, v2);
            if (pHit) {
                const auto dist = glm::distance(*pHit, pOrig);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = card;
                }
            }
        }
    }

    return nearest;
}
