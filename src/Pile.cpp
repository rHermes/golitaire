//
// Created by rhermes on 11/16/22.
//

#include "Pile.h"

using namespace gol;

Pile::Pile(const glm::vec3 &pos) : position_{pos} {
}

void Pile::addCard(std::shared_ptr<Card> card) {
    card->setPosition(position_ + static_cast<float>(cards_.size()) * glm::vec3(0.0f, -0.25f, 1.0f));
    cards_.push_back(std::move(card));
}

bool Pile::empty() const {
    return cards_.empty();
}

std::shared_ptr<Card> Pile::popTop() {
    if (cards_.empty()) {

    }
    std::shared_ptr<Card> tmp = cards_.back();
    cards_.pop_back();
    return tmp;
}
