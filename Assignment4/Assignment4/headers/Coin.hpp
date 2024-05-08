#pragma once

#include "Sprite.hpp"

class Coin : public Sprite {
public:
	Coin(const std::string& name);

	void update() override;
};