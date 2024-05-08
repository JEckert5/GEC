#include "Item.hpp"

Item::Item(Shader* shader, const Texture& texture)
	: Sprite(Square, 
			 shader,
			 texture,
			 1.0 / 6.0,
			 0.35,
			 6,
			 1,
			 0.2f
) {
	
}

