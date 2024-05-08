#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP
#include <vector>
#include <list>
#include <memory>

#include "Audio.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Text.hpp"

struct Scene {
	Scene() = default;
	~Scene() = default;

	void build();

	void update();
	void checkCollision();
	void applyGravity() const;
	void draw() const;

	void reset();

	Camera camera;

	std::vector<std::shared_ptr<Object>> children;
	std::vector<std::shared_ptr<Object>> ground;
	std::vector<std::shared_ptr<Object>> obstacles;
	std::vector<std::shared_ptr<Coin>> coins;
	std::vector<std::shared_ptr<Text>> text;

	float timerVal = 500;
	int score;
	float gravity = 0.5f;
	float cameraXBound = 0.3f;
	bool inMenu = true, paused = false, inVictory = false;

	std::shared_ptr<Player> player;
	std::shared_ptr<Object> goal;

	Audio* bgm, *collect, *win;
	std::shared_ptr<Text> menu, pause, victory;
};

#endif