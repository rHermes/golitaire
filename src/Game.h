//
// Created by rhermes on 9/11/22.
//

#pragma once

#include <vector>
#include "LTK/ResourceManager.h"
#include "SceneRenderer.h"
#include "Pile.h"
#include "Viewport.h"

namespace gol {

    enum class MouseButton {
        Right,
        Left
    };

class Game {
private:
    // These are viewport height and width
    LTK::Viewport vp_{1, 1};

    const float cardsWide_{9.0f};
    const float cardsTall_{4.7f};

    const glm::vec3 relationPos{0, cardsTall_-1, 10};
    const glm::vec3 rowRelPos = relationPos + glm::vec3(0, -1.2, 0);

    const glm::vec3 onePileRight{1 + (cardsWide_ - 7)/6, 0, 0};
    const glm::vec3 oneCardDown{0, -0.25, 0};
    const glm::vec3 oneLevelForward{0.0f, 0.0f, 1};

    std::shared_ptr<LTK::ResourceManager> resourceManager_;
    std::unique_ptr<SceneRenderer> sceneRenderer_;


    Pile drawPile_{relationPos, Pile::GrowthType::OnTop};
    Pile discardPile_{relationPos + onePileRight, Pile::GrowthType::Downward};

    std::vector<Pile> piles_;

    // The mouse position in screen space
    glm::vec2 mousePos_;
    glm::vec2 prevMousePos_;

    // Mouse buttons
    std::unordered_map<MouseButton,bool> mouseButtons_;

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

    void setMousePosition(float x, float y);

    void setMouseButtonState(MouseButton button, bool pressed);
    [[nodiscard]] bool getMouseButtonState(MouseButton button) const;

    // This redeals the hands
    void restartGame();
};

} // gol
