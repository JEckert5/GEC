#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <GL/glew.h>
#include "FreeImage/FreeImage.h"
#include <iostream>
#include <vector>

struct Texture {
	static Texture create(const std::string& path);
	GLuint id;
	Texture() = default;
	~Texture();

	[[nodiscard]] bool isValid() const {
		return id != 0;
	}

	inline static std::vector<Texture> TextureLib{};
};

#endif