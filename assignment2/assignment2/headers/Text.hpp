#pragma once

#ifndef TEXT_HPP
#define TEXT_HPP
#include <string>
#include <vector>
#include <GL/glew.h>

#include "Shader.hpp"
#include "Char.hpp"

class Text {
public:
	Text(std::string str, glm::vec2 pos, glm::vec3 color, Shader* shader, float scale = 1);
	Text() = default;

	void draw();

	std::string text;
	glm::vec2 position;
	float scale;
	glm::vec3 color;
private:
	GLuint mVAO, mVBO, mEBO;
	std::vector<GLuint> mIndices{};
	Shader* mShader = nullptr;
	glm::mat4 mProj;
};

#endif