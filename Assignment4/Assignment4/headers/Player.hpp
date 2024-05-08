#pragma once

#include "Enemy.hpp"
#include "Sprite.hpp"

class Player : public Sprite {
public:
	Player();

	void update() override;
	std::string saveString() override;

	void physical(Enemy* target);
	void magic(Enemy* target);

	float health, mana;
	float speed;
	float jumpHeight;
	float yVel = 0.f;
	bool inCombat = false;
	bool blockedLeft, blockedRight, blockedTop, blockedBottom;
};