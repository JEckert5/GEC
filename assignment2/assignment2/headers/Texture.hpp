#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <GL/glew.h>
#include "FreeImage/FreeImage.h"
#include <iostream>

struct Texture {
	Texture(const std::string& path);
	Texture() = default;

	GLuint id = 0;
};

#endif