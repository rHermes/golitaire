//
// Created by rhermes on 9/11/22.
//

#include "Game.h"

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
    const double furtherOn = dt * (std::numbers::pi/30.0);
    singleCard_->rotateY(static_cast<float>(furtherOn));
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // We run this once here, but we should do it somewhere else?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We want to cull
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    sceneRenderer_->render(vpWidth, vpHeight);
}

void Game::init() {
    glClearColor(1.0f, 0.0f, 0.77f, 1.0f);
    glViewport(0, 0, vpWidth, vpHeight);

    resourceManager_ = std::make_shared<LTK::ResourceManager>();
    sceneRenderer_ = std::make_unique<SceneRenderer>(resourceManager_);

    singleCard_ = std::make_shared<Card>(CardSuite::Diamonds, CardMember::King);
    singleCard_->setScale(0.05f);

    sceneRenderer_->addCard(singleCard_);
}
