//
// Created by rhermes on 9/11/22.
//

#include "Game.h"

#include <numbers>
#include <glad/gles2.h>

using namespace gol;

void Game::resizeViewport(int width, int height) {
    vpWidth = width;
    vpHeight = height;
    glViewport(0, 0, width, height);


}


void Game::processInput(const double dt) {
}

void Game::update(const double dt) {
    // const double furtherOn = dt * ((2.0*std::numbers::pi)/10);
    // singleCard_->rotateY(static_cast<float>(furtherOn));
    for (int i = 52; 0 < i; i--) {
        const auto& card = deck_[i-1];
        glm::vec3 pos = card->getPosition();
        pos.x += static_cast<float>(dt*(10.0/5));

        card->setPosition(pos);
        if (pos.x < -0.5f)
            break;
    }
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);


    sceneRenderer_->render(vpWidth, vpHeight);
}

void Game::init() {
    glClearColor(1.0f, 0.0f, 0.77f, 1.0f);
    glViewport(0, 0, vpWidth, vpHeight);

    resourceManager_ = std::make_shared<LTK::ResourceManager>();
    sceneRenderer_ = std::make_unique<SceneRenderer>(resourceManager_);

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
        card->setScale(0.03f);
        card->setPosition({-2, 0, i*0.01});

        deck_.push_back(card);
        sceneRenderer_->addCard(card);
    }

    // We run this once here, but we should do it somewhere else?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We want to cull
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}
