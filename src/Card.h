//
// Created by rhermes on 11/2/22.
//

#pragma once

#include "CardEnums.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "SceneObject.h"


namespace gol {

class Card : public SceneObject {
private:
    const CardType type_;

public:
    Card(CardSuite suite, CardMember member);

    [[nodiscard]] CardSuite getSuite() const;
    [[nodiscard]] CardMember getMember() const;
    [[nodiscard]] CardType getType() const;
};

} // gol
