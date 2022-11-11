//
// Created by rhermes on 11/2/22.
//

#pragma once

#include "CardEnums.h"
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace gol {

class Card {
private:
    const CardType type_;

    // X, Y, Z
    glm::vec3 rotation_{0, 0, 0};
    glm::vec3 position_{0, 0, 0};
    float scale_{1.0};

    glm::mat4 transform_;
    bool transform_dirty_{true};


    // This is not really a const operation, but the semantics are const
    void updateTransform();

public:

    Card(CardSuite suite, CardMember member);

    [[nodiscard]] const glm::mat4& getTransform();

    void setScale(float scale);
    void setPosition(glm::vec3 pos);

    // Rotation matrixes
    void setRotationX(float radians);
    void rotateX(float radians);

    void setRotationY(float radians);
    void rotateY(float radians);

    void setRotationZ(float radians);
    void rotateZ(float radians);

    [[nodiscard]] CardSuite getSuite() const;
    [[nodiscard]] CardMember getMember() const;
    [[nodiscard]] CardType getType() const;

    [[nodiscard]] const glm::vec3& getPosition() const;
};

} // gol
