//
// Created by rhermes on 10/9/22.
//

#pragma once

#include <utility>
#include "utils.h"
#include <stdexcept>

namespace gol {
    enum class CardSuite {
        Joker,
        Clubs,
        Spades,
        Diamonds,
        Hearts
    };

    enum class CardMember {
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace,
        RedJoker,
        BlackJoker
    };

    struct CardType {
        bool operator==(const CardType &rhs) const {
            return std::tie(suite, member) == std::tie(rhs.suite, rhs.member);
        }

        bool operator!=(const CardType &rhs) const {
            return !(rhs == *this);
        }

        CardSuite suite;
        CardMember member;

        CardType(CardSuite su, CardMember mem) : suite{su}, member{mem} {
            // We see if it's valid.
            if (suite == CardSuite::Joker) {
                if (member != CardMember::RedJoker && member != CardMember::BlackJoker)
                    throw std::invalid_argument("A joker suite can only have red or black joker");
            } else {
                if (member == CardMember::RedJoker || member == CardMember::BlackJoker)
                    throw std::invalid_argument("A normal suite can not be a joker.");
            }
        }
    };

} // gol

namespace std {
    template <> struct hash<gol::CardType>
    {
        size_t operator()(const gol::CardType& x) const
        {
            size_t seed = 0;
            gol::utils::hash_combine(seed, x.suite);
            gol::utils::hash_combine(seed, x.member);
            return seed;
        }
    };
}

