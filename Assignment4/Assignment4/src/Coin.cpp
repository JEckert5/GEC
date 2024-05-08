#include "Coin.hpp"

void Coin::update() {
	Sprite::update();
}

Coin::Coin(const std::string& name): Sprite(name) {
	type = COIN;

	Animation spin("textures/coinSheet.png", 6, 5, 0, 0, 1.0 / 6.0, 0.35);

	m_animManager.addAnimation("default", spin);
	m_animManager.setState("default");

	boxCollider.SetAsBox(0.25f, 0.25f);
}
