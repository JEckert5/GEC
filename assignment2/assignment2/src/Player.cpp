#include "Player.hpp"

Player::Player(Shader* shader)
	: Sprite(
		Square, 
		shader,
		{ "textures/spriteSheet.png" }, 
		1.0 / 16.0, 
		1.0 / 16.0,
		8, 
		1,
		0.2f,
		2)
{
	
}
