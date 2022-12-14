//
// Created by rhermes on 11/16/22.
//

#pragma once

#include <vector>
#include "glm/vec3.hpp"
#include "Card.h"

namespace gol {

    /**
     * A pile of cards.
     */
    class Pile {
    private:
        glm::vec3 position_{0};
        std::vector<std::shared_ptr<Card>> cards_;

    public:
       explicit Pile(const glm::vec3& pos);

       void addCard(std::shared_ptr<Card> card);

       [[nodiscard]] bool empty() const;
       [[maybe_unused]] std::shared_ptr<Card> popTop();
    };

} // gol
