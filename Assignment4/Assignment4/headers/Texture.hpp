#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <unordered_map>
#include <vector>

struct Texture {
	static Texture create(const std::string& path);
	static void destroy(Texture& texture);
	GLuint id;
	std::string fileName;
	Texture() = default;
	Texture(const Texture& other);

	Texture& operator=(const Texture& rhs);


	[[nodiscard]] bool isValid() const {
		return id != 0;
	}

	inline static std::unordered_map<std::string, Texture> TextureLib{};
};

#endif