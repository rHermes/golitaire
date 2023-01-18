//
// Created by rhermes on 11/2/22.
//

#include "Card.h"
#include <glm/gtc/quaternion.hpp>

using namespace gol;


Card::Card(CardSuite suite, CardMember member) : type_{suite, member} {
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

void Card::setFaceup(bool yes) {
    if (yes != faceup_) {
        if (yes)
            setRotation(0, 0, 0);
        else
            setRotation(0, glm::radians(180.0f), 0);

        faceup_ = yes;
    }
}

bool Card::isFaceup() const {
    return faceup_;
}

