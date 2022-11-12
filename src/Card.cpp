//
// Created by rhermes on 11/2/22.
//

#include "Card.h"
#include "glm/ext/matrix_transform.hpp"
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

