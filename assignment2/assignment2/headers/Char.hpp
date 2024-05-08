#pragma once

#ifndef CHAR_HPP
#define CHAR_HPP
#include <map>
#include <glm/glm.hpp>

struct Char {
	unsigned int texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;

	inline static std::map<char, Char> Chars{};
};

#endif
