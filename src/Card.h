//
// Created by rhermes on 11/2/22.
//

#pragma once

#include "CardEnums.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "LTK/Item3D.h"


namespace gol {

class Card : public LTK::Item3D {
private:
    const CardType type_;
    bool faceup_{true};

public:
    Card(CardSuite suite, CardMember member);

    [[nodiscard]] CardSuite getSuite() const;
    [[nodiscard]] CardMember getMember() const;
    [[nodiscard]] CardType getType() const;

    void setFaceup(bool yes);
    [[nodiscard]] bool isFaceup() const;
};

} // gol
