#pragma once

#include "Sprite.hpp"

#include <random>

class Player;

class Enemy : public Sprite {
public:
	Enemy(const std::string& name);

	void update() override;



	float health;
	bool blockedLeft, blockedRight, blockedTop, blockedBottom;

	std::shared_ptr<Player> target;

	void putInBattle();

private:
	enum class Cycle {
		WALK,
		IDLE
	};

	enum class Facing {
		UP,
		RIGHT,
		DOWN ,
		LEFT,
	};

	bool m_inBattle = false;
	Facing m_direction;
	Cycle m_currentCycle = Cycle::IDLE;
	float m_currentCycleTime = 0;
	float m_cycleLifetime = 0;

	float m_attackTimer = 0;
	float m_attackInterval = 1.34f;

	std::random_device m_randomInitializer{};
	std::mt19937 m_random;
};
