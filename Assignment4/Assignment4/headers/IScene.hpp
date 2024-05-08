#pragma once

#include <vector>
#include <memory>

#include "Audio.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Text.hpp"

struct IScene {
	virtual void build() = 0;
	virtual void update() = 0;
	virtual void checkCollision() = 0;
	virtual void draw() = 0;
	virtual void reset() = 0;
	virtual void clear() = 0;
	virtual void start() = 0;
	virtual void pause() = 0;

	Camera camera;

	// Everything drawn goes in here.
	std::vector<std::shared_ptr<Object>> drawables;

	std::vector<std::shared_ptr<Text>> ui;
};