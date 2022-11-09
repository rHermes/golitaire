//
// Created by rhermes on 11/2/22.
//

#include "Card.h"
#include "glm/ext/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>

using namespace gol;


Card::Card(CardSuite suite, CardMember member) : type_{suite, member} {
}

const glm::mat4 &Card::getTransform() {
    // We are a bit naughty here, and call the update transform here.
    if (transform_dirty_)
        updateTransform();

    return transform_;
}

void Card::updateTransform() {
    transform_ = glm::mat4(1.0f);


    transform_ = glm::translate(transform_, position_);

    // This is what I currently have, might not be correct.
    glm::quat qt;
    qt = glm::rotate(qt, rotation_.x, {1.0f, 0.0f, 0.0f});
    qt = glm::rotate(qt, rotation_.y, {0.0f, 1.0f, 0.0f});
    qt = glm::rotate(qt, rotation_.z, {0.0f, 0.0f, 1.0f});

    transform_ *= static_cast<glm::mat4>(qt);
    transform_ = glm::scale(transform_, glm::vec3(scale_));

    transform_dirty_ = false;
}

void Card::setRotationX(float radians) {
    rotation_.x = radians;
    transform_dirty_ = true;
}

void Card::setRotationY(float radians) {
    rotation_.y = radians;
    transform_dirty_ = true;
}

void Card::setRotationZ(float radians) {
    rotation_.z = radians;
    transform_dirty_ = true;
}

void Card::rotateX(float radians) {
    rotation_.x += radians;
    transform_dirty_ = true;
}

void Card::rotateY(float radians) {
    rotation_.y += radians;
    transform_dirty_ = true;
}

void Card::rotateZ(float radians) {
    rotation_.z += radians;
    transform_dirty_ = true;
}

CardSuite Card::getSuite() const {
    return type_.suite;
}

CardMember Card::getMember() const {
    return type_.member;
}

CardType Card::getType() const {
    return type_;
}

void Card::setScale(float scale) {
    scale_ = scale;
    transform_dirty_ = true;
}

void Card::setPosition(glm::vec3 pos) {
    position_ = std::move(pos);
    transform_dirty_ = true;
}


