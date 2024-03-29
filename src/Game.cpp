//
// Created by rhermes on 9/11/22.
//

#include "Game.h"

#include <numbers>
#include <algorithm>
#include <glad/gles2.h>
#include <spdlog/spdlog.h>

using namespace gol;

void Game::resizeViewport(int width, int height) {
    vp_.setWidth(width);
    vp_.setHeight(height);

    vp_.apply();

    if (sceneRenderer_) {
        sceneRenderer_->setWindowSize(width, height);
    }
}


void Game::processInput(const double dt) {
    if (getMouseButtonState(MouseButton::Left)) {
        // Ok, so we are here, let's try to remove the card we intersect with
        const auto hCard = sceneRenderer_->hitTestCards(mousePos_);
        if (hCard) {
            spdlog::info("We hit a card!");
        }
    }


    // We update the mouse position
    prevMousePos_ = mousePos_;
}

void Game::update(const double dt) {
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sceneRenderer_->render();
}

void Game::init() {
    glClearColor(1.0f, 0.0f, 0.77f, 1.0f);
    vp_.apply();
    // We need depth testing
    glEnable(GL_DEPTH_TEST);

    // We run this once here, but we should do it somewhere else?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We want to cull
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    resourceManager_ = std::make_shared<LTK::ResourceManager>();
    sceneRenderer_ = std::make_unique<SceneRenderer>(resourceManager_);

    sceneRenderer_->setCardsTall(cardsTall_);
    sceneRenderer_->setCardsWide(cardsWide_);
    sceneRenderer_->setWindowSize(vp_.width(), vp_.height());

    // We create the deck of cards.
    std::vector<CardSuite> suits{{CardSuite::Spades, CardSuite::Diamonds, CardSuite::Clubs, CardSuite::Hearts}};
    std::vector<CardMember> members{{
        CardMember::Ace, CardMember::Two, CardMember::Three, CardMember::Four,
        CardMember::Five, CardMember::Six, CardMember::Seven, CardMember::Eight,
        CardMember::Nine, CardMember::Ten, CardMember::Jack, CardMember::Queen,
        CardMember::King
    }};
    for (int i = 0; i < 52; i++) {
        const auto suiteIdx = i / members.size();
        const auto memberIdx = i % members.size();

        auto card = std::make_shared<Card>(suits[suiteIdx], members[memberIdx]);

        drawPile_.addCard(card);
        sceneRenderer_->addCard(card);
    }

    // We need to add 7 piles.
    for (auto i = 0; i < 7; i++) {
        const auto pos = rowRelPos + static_cast<float>(i)*onePileRight;
        piles_.emplace_back(pos);
    }

    restartGame();
}


void Game::restartGame() {
    while (!discardPile_.empty()) {
        auto card = discardPile_.popTop();
        card->setFaceup(false);
        drawPile_.addCard(std::move(card));
    }

    // We move all card from the piles into the deck.
    for (auto& pile : piles_) {
        while (!pile.empty()) {
            auto card = pile.popTop();
            card->setFaceup(false);
            drawPile_.addCard(std::move(card));
        }
    }

    // const float distVert = 0.1;

    // Now we shuffle the deck.
    drawPile_.shuffle();

    for (int row = 0; row < 7; row++) {
        for (int pile = row; pile < 7; pile++) {
            auto card = drawPile_.popTop();

            card->setFaceup(false);
            piles_[pile].addCard(card);
        }
    }

    // For each front card in each pile we turn it up
    for (const auto& pile : piles_) {
        pile.top()->setFaceup(true);
    }

    // we move the next card to the top spot
    {
        auto card = drawPile_.popTop();
        card->setFaceup(true);
        discardPile_.addCard(std::move(card));
    }

    for (const auto& card : drawPile_) {
        card->setFaceup(false);
    }
}

void Game::setMousePosition(const float x, const float y) {
    mousePos_ = {x, y};
}

void Game::setMouseButtonState(MouseButton button, bool pressed) {
    // spdlog::debug("We got a mouse event {} {}", static_cast<int>(button), pressed);
    mouseButtons_[button] = pressed;
}

bool Game::getMouseButtonState(const MouseButton button) const {
    const auto it = mouseButtons_.find(button);
    if (it != std::cend(mouseButtons_)) {
        return it->second;
    } else {
        return false;
    }
}
