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

}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::init() {
    glClearColor(1.0f, 0.0f, 0.77f, 1.0f);
    glViewport(0, 0, vpWidth, vpHeight);

    resourceManager_ = std::make_unique<LTK::ResourceManager>();

    auto awesomeFace = resourceManager_->loadTexture("data/textures/awesomeface.png", true);
    auto wall = resourceManager_->loadTexture("data/textures/wall.jpg", false);
    auto container = resourceManager_->loadTexture("data/textures/container.jpg", false);

    auto defaultProg = resourceManager_->loadShaderProgram(
            "data/shaders/default.vert", "data/shaders/default.frag"
            );
}
