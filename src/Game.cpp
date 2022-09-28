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
}
