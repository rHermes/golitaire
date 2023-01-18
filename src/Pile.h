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
    public:
        enum class GrowthType {
            Downward,
            OnTop
        };

    private:
        glm::vec3 position_{0};
        std::vector<std::shared_ptr<Card>> cards_;
        GrowthType growthType_{GrowthType::Downward};

        [[nodiscard]] static glm::vec3 getDelta(GrowthType growthType);


        void repositionCards();

    public:
       explicit Pile(const glm::vec3& pos);

       Pile(const glm::vec3& pos, GrowthType growthType);

       void setGrowthType(GrowthType growthType);

       void addCard(std::shared_ptr<Card> card);

       // Throws exception, if card is not present
       void removeCard(const std::shared_ptr<Card>& card);

       [[nodiscard]] bool empty() const;
       [[maybe_unused]] std::shared_ptr<Card> popTop();

       [[nodiscard]] bool hasCard(const std::shared_ptr<Card>& card) const;

       [[nodiscard]] const glm::vec3& getPosition() const;
       void setPosition(const glm::vec3& position);


       void shuffle();

       [[nodiscard]] const std::shared_ptr<Card>& top() const;

       // To allow for iteration
       [[nodiscard]] auto begin() { return cards_.begin(); };
       [[nodiscard]] auto end() { return cards_.end(); }
       [[nodiscard]] auto cbegin() const { return cards_.cbegin(); }
       [[nodiscard]] auto cend() const { return cards_.cend(); }
    };

} // gol
