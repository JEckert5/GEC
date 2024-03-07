#include "Player.hpp"

Player::Player(Shader* shader) : Object(Square, shader, "textures/player.png") { }


void Player::update(const int deltaTime, const int elapsedTime) {
	// Take input

	Object::update(deltaTime, elapsedTime); // Update transform matrix
}
