#pragma once

#include "IScene.hpp"
#include "Player.hpp"

#include <box2d/b2_chain_shape.h>

#include "Coin.hpp"
#include "Enemy.hpp"
#include "HealthBar.hpp"
#include "Panel.hpp"

struct FFScene : IScene {
	FFScene() = default;
	~FFScene() = default;

	void build() override;
	void update() override;
	void checkCollision() override;
	void draw() override;
	void reset() override;
	void clear() override;
	void start() override;
	void pause() override;
	void click(int x, int y);

	bool inMenu = true, paused = false, inVictory = false, gameOver = false;
	bool battleMode = false;
	int score = 0;
	std::shared_ptr<Text> title, press, pauseText, win, gameOverTxt;
	std::shared_ptr<Text> health, mana, howBattle, scoreTxt;

	glm::vec3 playerPosBeforeBattle;

	Panel battleOptions;

	b2ChainShape border;

	std::vector<std::shared_ptr<Object>> ground;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Coin>> coins;
	std::vector<HealthBar> healthBars;

	std::vector<std::shared_ptr<Enemy>>::iterator targetIterator;
	std::vector<std::shared_ptr<Object>>::iterator drawderator;

	std::shared_ptr<Player> player;

	Audio victorySound, bgm, winSound, battle;
};
