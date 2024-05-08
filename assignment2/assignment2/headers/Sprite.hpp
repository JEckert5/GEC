#pragma once

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Object.hpp"

class Sprite: public Object {
public:
	Sprite() = default;
	Sprite(const Mesh& mesh, Shader* shader, const Texture& spriteSheet, double xOffset, double yOffset, int columns, int rows, float interval, int baseRow = 1);

	void update(const double deltaTime, const double elapsedTime) override;
	void draw(const glm::mat4& view, const glm::mat4& proj) override;

protected:
	double mXOffset{}; // Right
	double mYOffset{}; // Down
	int mColumns{};
	int mRows{};
	float mInterval{};
	int mCurrentColumn = 0;
	int mCurrentRow = 0;
	int mBaseRow = 1;

	void setupBuffers() override;
};

#endif