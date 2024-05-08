#include "Enemy.hpp"

#include "Player.hpp"
#include "Time.hpp"

Enemy::Enemy(const std::string& name):
Sprite(name),
health(100),
blockedLeft(false),
blockedRight(false),
blockedTop(false),
blockedBottom(false),
m_random(m_randomInitializer()) {
	type = ENEMY;

	const Animation idle("textures/enemySheet.png", 1, 1, 0, 0.38, 1.0 / 18.6, 1.0 / 4.8);
	const Animation walk("textures/enemySheet.png", 5, 5, 0.11, 0.38, 1.0 / 18.6, 1.0 / 4.8);

	m_animManager.addAnimation("idle", idle);
	m_animManager.addAnimation("walk", walk);

	m_animManager.setState("walk");

	scale = { 0.5f, 0.5f, 1 };

	boxCollider.SetAsBox(0.25f, 0.25f);
}

void Enemy::update() {
	Sprite::update();

	m_currentCycleTime += Time::deltaTime;

	if (m_inBattle) {

		m_attackTimer += Time::deltaTime;

		if (m_attackTimer >= m_attackInterval && target != nullptr) {
			target->health -= m_random() % 5;
			m_attackTimer = 0;
		}

		return;
	}

	if (m_currentCycle == Cycle::WALK) {
		switch (m_direction) {
		case Facing::UP:
			if (!blockedTop)
				position.y += Time::deltaTime / 5.f;
			break;
		case Facing::RIGHT:
			if (!blockedRight)
				position.x += Time::deltaTime / 5.f;
			break;
		case Facing::LEFT:
			if (!blockedLeft)
				position.x -= Time::deltaTime / 5.f;
			break;
		case Facing::DOWN:
			if (!blockedBottom)
				position.y -= Time::deltaTime / 5.f;
			break;
		}
	}

	if (m_currentCycleTime < m_cycleLifetime)
		return;

	m_currentCycleTime = 0;

	switch (m_currentCycle) {
	case Cycle::WALK:
		m_currentCycle = Cycle::IDLE;
		m_cycleLifetime = 3.f;

		m_animManager.setState("idle");
		break;
	case Cycle::IDLE:
		m_currentCycle = Cycle::WALK;
		m_cycleLifetime = 5.f;

		m_direction = static_cast<Facing>(m_random() % 4);

		m_animManager.setState("walk");

		/*if (m_direction == Facing::LEFT)
			scale.x = -scale.x;*/
		break;
	}
}

void Enemy::putInBattle() {
	m_inBattle = true;
	m_animManager.setState("idle");
}
