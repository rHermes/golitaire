//
// Created by rhermes on 9/11/22.
//

#pragma once

#include <vector>
#include "LTK/ResourceManager.h"
#include "SceneRenderer.h"

namespace gol {

class Game {
private:
    // These are viewport height and width
    int vpWidth{1};
    int vpHeight{1};

    float cardsWide_{9.0f};
    float cardsTall_{4.7f};

    std::shared_ptr<LTK::ResourceManager> resourceManager_;
    std::unique_ptr<SceneRenderer> sceneRenderer_;

    // std::shared_ptr<Card> singleCard_;
    std::vector<std::shared_ptr<Card>> deck_;

public:
    // Initialize all game resources and so on
    void init();

    // Indicate that the viewport has been resized.
    void resizeViewport(int width, int height);

    // Process the input. This is really an update of the game state based on the
    // active inputs.
    void processInput(double dt);

    // Update the game state. This is more in the sense of score and so on.
    void update(double dt);

    // Render. This is a static function, which doesn't modify any of the classes state, except for
    // things having to do with rendering.
    void render();


    // This redeals the hands
    void restartGame();
};

} // gol
