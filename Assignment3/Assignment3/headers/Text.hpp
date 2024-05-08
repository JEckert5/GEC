#pragma once

#ifndef TEXT_HPP
#define TEXT_HPP
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "Shader.hpp"
#include "Char.hpp"
#include "Config.hpp"
#include "Object.hpp"

class Text {
public:
	Text(std::string str, glm::vec2 pos, glm::vec3 color, float scale = 1, const glm::mat4& proj = glm::ortho(0.0, Config::WindowW, 0.0, Config::WindowH));
	Text() = default;

	void draw() const;
	void setProjection(const glm::mat4& newProj);

	std::string str;
	glm::vec2 position;
	float scale;
	glm::vec3 color;
	bool active = true;
private:

	GLuint mVAO, mVBO, mEBO;
	std::vector<GLuint> mIndices = {
		0, 1, 2,
		0, 2, 3
	};
	glm::mat4 mProj;
};

#endif