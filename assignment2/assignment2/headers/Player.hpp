#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Object.hpp"
#include "Sprite.hpp"

class Player : public Sprite {
public:
    Player() = default;
    Player(Shader* shader);

    // void update(const double deltaTime, const double elapsedTime) override;
};

#endif