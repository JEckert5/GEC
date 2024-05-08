#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>

struct Char {
	unsigned int texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;

	inline static std::map<std::string, std::map<char, Char>> Fonts{};
};