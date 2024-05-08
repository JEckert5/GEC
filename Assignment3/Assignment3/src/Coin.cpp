#include "Object.hpp"

void Coin::update() {
	Sprite::update();
}

Coin::Coin(): Sprite("coin", 6, 1, 1.0 / 6.0, 0.35, 0.2f, {Texture::create("textures/coinSheet.png")}) {
	type = ObjectType::COLLECTABLE;
}
