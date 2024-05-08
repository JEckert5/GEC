#pragma once

#ifndef ITEM_HPP
#define ITEM_HPP
#include "Sprite.hpp"

class Item : public Sprite {
public:
	Item() = default;
	Item(Shader* shader, const Texture& texture);
};

#endif