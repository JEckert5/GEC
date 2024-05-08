// #include <GL/freeglut.h>
#include <Windows.h>

#include "Object.hpp"
#include "Time.hpp"

Player::Player(float speed, float jh)
	: Sprite("Player",
			 8, 2,
			 1.0 / 16.0, 1.0 / 16.0, 
			 0.2f, 
			 { Texture::create("textures/spriteSheet.png") }),
	speed(speed),
	jumpHeight(jh),
	jumped(false)
{
	type = ObjectType::PLAYER;

	collider = glm::vec3(0.2f, 0.25f, 0.5f);
	colliderOffset.y = -0.05f;
}

void Player::update() {
	Sprite::update();

	const auto frameGrav = gravity * Time::deltaTime * Time::deltaTime;

	if (GetAsyncKeyState(VK_LEFT) && !blockedLeft)
		position.x -= speed * Time::deltaTime;

	if (GetAsyncKeyState(VK_RIGHT) && !blockedRight)
		position.x += speed * Time::deltaTime;

	if (GetAsyncKeyState(VK_SPACE) && !jumped && grounded) {
		yVel = sqrtf(jumpHeight * gravity);
		jumped = true;
	}

	if ((grounded && !jumped) || !hasGravity) {
		yVel = 0;
	} else {
		yVel -= frameGrav;
	}

	if (jumped)
		jumped = false;

	position.y += yVel;
}
