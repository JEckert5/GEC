#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Object.hpp"

class Player: public Object {
public:
	Player(Shader* shader);

	void update(const int deltaTime, const int elapsedTime) override;
};

#endif