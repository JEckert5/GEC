#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include "IScene.hpp"
#include "Player.hpp"
#include "Coin.hpp"

struct MainScene: IScene {
	MainScene() = default;
	~MainScene() = default;

	void build() override;

	void update() override;
	void checkCollision() override;
	void applyGravity() const;
	void draw() override;

	void reset() override;

	bool inMenu, paused, inVictory;
	std::shared_ptr<Text> menu, pause, victory;

	std::vector<std::shared_ptr<Object>> ground;
	std::vector<std::shared_ptr<Object>> obstacles;
	std::vector<std::shared_ptr<Coin>> coins;
	std::vector<std::shared_ptr<Text>> text;

	float timerVal = 500;
	int score;
	float gravity = 0.5f;
	float cameraXBound = 0.3f;

	std::shared_ptr<Player> player;
	std::shared_ptr<Object> goal;

	Audio* bgm, *collect, *win;

};

#endif