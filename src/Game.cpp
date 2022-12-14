//
// Created by rhermes on 9/11/22.
//

#include "Game.h"

#include <numbers>
#include <algorithm>
#include <random>
#include <glad/gles2.h>

using namespace gol;

void Game::resizeViewport(int width, int height) {
    vpWidth = width;
    vpHeight = height;
    glViewport(0, 0, width, height);


    // We also need to reposition the cards.
}


void Game::processInput(const double dt) {
}

void Game::update(const double dt) {
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    sceneRenderer_->render(vpWidth, vpHeight, cardsWide_, cardsTall_);
}

void Game::init() {
    glClearColor(1.0f, 0.0f, 0.77f, 1.0f);
    glViewport(0, 0, vpWidth, vpHeight);

    // We need depth testing
    // glEnable(GL_DEPTH_TEST);

    // We run this once here, but we should do it somewhere else?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We want to cull
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    resourceManager_ = std::make_shared<LTK::ResourceManager>();
    sceneRenderer_ = std::make_unique<SceneRenderer>(resourceManager_);

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

        deck_.push_back(card);
        sceneRenderer_->addCard(card);
    }

    restartGame();
}


void Game::restartGame() {
    // First we shuffle the deck
    std::random_device rd;
    std::mt19937 g(rd());

    // We move all card from the piles into the deck.
    /*
    for (auto& pile : piles_) {
    }
     */

    std::shuffle(deck_.begin(), deck_.end(), g);
    // const float distVert = 0.1;

    auto it = std::begin(deck_);
    for (int row = 0; row < 7; row++) {
        for (int pile = row; pile < 7; pile++) {
            auto& card = *it;
            it++;

            card->setFaceup(pile == row);

            const auto pos = rowRelPos + static_cast<float>(pile)*onePileRight +
                    static_cast<float>(row)*(oneCardDown + oneLevelForward);

            card->setPosition(pos);
        }
    }

    // we move the next card to the top spot
    {
        auto& card = *it;
        it++;

        card->setPosition(relationPos + onePileRight);
        card->setFaceup(true);
    }

    // The remaining cards are moved to the pile and swapped.
    for (; it != std::end(deck_); ++it) {
        auto& card = *it;

        card->setFaceup(false);
        card->setPosition(relationPos);
    }
}
