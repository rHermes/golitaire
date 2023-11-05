//
// Created by rhermes on 11/16/22.
//

#include <random>
#include <algorithm>
#include "Pile.h"

using namespace gol;

Pile::Pile(const glm::vec3 &pos) : position_{pos} {
}


Pile::Pile(const glm::vec3 &pos, Pile::GrowthType growthType) : position_(pos), growthType_(growthType) {}

void Pile::addCard(std::shared_ptr<Card> card) {
    const auto delta = getDelta(growthType_);
    card->setPosition(position_ + static_cast<float>(cards_.size()) * delta);

    cards_.push_back(std::move(card));
}

bool Pile::empty() const {
    return cards_.empty();
}

std::shared_ptr<Card> Pile::popTop() {
    if (cards_.empty()) {
        throw std::runtime_error("Tried to pop top card of empty stack!");
    }
    std::shared_ptr<Card> tmp = cards_.back();
    cards_.pop_back();
    return tmp;
}

const std::shared_ptr<Card> &Pile::top() const {
    if (cards_.empty()) {
        throw std::runtime_error("Tried to fetch top card from an empty stack");
    }

    return cards_.back();
}

void Pile::setGrowthType(const Pile::GrowthType growthType) {
    growthType_ = growthType;
}

void Pile::repositionCards() {
    const auto delta = getDelta(growthType_);
    for (decltype(cards_)::size_type i = 0; i < cards_.size(); i++) {
        cards_[i]->setPosition(position_ + static_cast<float>(i) * delta);
    }
}

glm::vec3 Pile::getDelta(Pile::GrowthType growthType) {
    switch (growthType) {
        case Pile::GrowthType::Downward:
            return {0.0f, -0.25f, 1.0f};
        case Pile::GrowthType::OnTop:
            return {0.0f, 0.0f, 1.0f};
    }

    throw std::invalid_argument("We have provided an invalid growthType");

}

void Pile::setPosition(const glm::vec3 &position) {
    position_ = position;
    repositionCards();
}

const glm::vec3 &Pile::getPosition() const {
    return position_;
}

void Pile::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(cards_.begin(), cards_.end(), g);

    repositionCards();
}


void Pile::removeCard(const std::shared_ptr<Card>& card) {
    const auto it = std::find(std::cbegin(cards_), std::cend(cards_), card);
    if (it == std::cend(cards_)) {
        throw std::out_of_range("The pile does not have the car");
    }
    
    cards_.erase(it);
}

bool Pile::hasCard(const std::shared_ptr<Card>& card) const {
    const auto it = std::find(std::cbegin(cards_), std::cend(cards_), card);
    return it != std::cend(cards_);
}