// #include <GL/freeglut.h>
#include <Windows.h>
#include <sstream>

#include "Player.hpp"
#include "Time.hpp"

Player::Player()
	: Sprite("Player"),
	health(100),
	mana(100),
	speed(0.7f),
	jumpHeight(0),
	blockedLeft(false),
	blockedRight(false),
	blockedTop(false),
	blockedBottom(false)
{
	type = PLAYER;

	collider = glm::vec3(0.2f, 0.25f, 0.5f);
	colliderOffset.y = -0.05f;

	hasGravity = false;

	const auto cs = Texture::create("textures/cliveSheet.png");

	const Animation idle(cs, 4, 2.4f, 0, 0, 1.0 / 16.0, 1.0 / 16.0);
	const Animation walk(cs, 8, 7, 0, 1.0 / 16.0, 1.0 / 16.0, 1.0 / 16.0);

	m_animManager.addAnimation("walk", walk);
	m_animManager.addAnimation("idle", idle);
	m_animManager.setState("idle");

	boxCollider.SetAsBox(0.1f, 0.15f, { 0, -0.05f }, 0);
}

void Player::update() {
	Sprite::update();

	m_animManager.setState("idle");

	if (inCombat)
		return;

	if (GetAsyncKeyState('A') && !blockedLeft) {
		position.x -= speed * Time::deltaTime;
		m_animManager.setState("walk");
	}

	if (GetAsyncKeyState('D') && !blockedRight) {
		position.x += speed * Time::deltaTime;
		m_animManager.setState("walk");
	}

	if (GetAsyncKeyState('W') && !blockedTop) {
		position.y += speed * Time::deltaTime;
		m_animManager.setState("walk");
	}

	if (GetAsyncKeyState('S') && !blockedBottom) {
		position.y -= speed * Time::deltaTime;
		m_animManager.setState("walk");
	}
}

std::string Player::saveString() {
	auto ball = Object::saveString();

	std::cout << ball << "\n";

	std::stringstream stream(ball);

	// set output position to end of the stream.
	stream.seekp(0, std::ios_base::end);

	stream << " " << jumpHeight << " " << speed;

	std::cout << stream.str() << "\n";

	return stream.str();
}

void Player::physical(Enemy* target) {
	target->health -= 5;
}

void Player::magic(Enemy* target) {
	if (mana >= 5) {
		target->health -= 10;
		mana -= 5;
	}
}

