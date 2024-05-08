#include "FFScene.hpp"

#include <box2d/b2_edge_shape.h>

void FFScene::build() {
	auto groundTexture = Texture::create("textures/ground.png");
	auto obstacleTexture = Texture::create("textures/obstacle.png");

	// Open World
	for (int y = 0; y < 6; y++) {
		for (int x = 0; x < 6; x++) {
			auto g = std::make_shared<Object>(Object::create("ground" + std::to_string(ground.size() + 1), { groundTexture }, DefaultMesh::Square));
			g->position.x = x;
			g->position.y = y;
			g->scale.x = 2;
			g->scale.y = 2;
			g->collider = glm::vec3(0);

			drawables.emplace_back(g);
			ground.emplace_back(g);
		}
	}

	for (int y = -1; y < 7; y++) {
		for (int x = -1; x < 7; x++) {
			if (y == -1 || y == 6 || (x < 0 || x > 5)) {
				auto o = std::make_shared<Object>(Object::create("perim" + std::to_string(x), { obstacleTexture }, DefaultMesh::SpriteSquare));
				o->position = { x, y, 0 };
				o->scale = { 2, 2, 1 };
				o->collider = glm::vec3(0);

				drawables.emplace_back(o);
			}
		}
	}

	// Battle Ground
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 2; x++) {
			auto g = std::make_shared<Object>(Object::create("battleGround" + std::to_string(x + y), { obstacleTexture }, DefaultMesh::Square));

			g->position.x = x - 10;
			g->position.y = y - 10;
			g->scale.x = 2;
			g->scale.y = 2;
			g->collider = glm::vec3(0);

			drawables.emplace_back(g);
		}
	}

	player = std::make_shared<Player>();

	drawables.emplace_back(player);

	auto e = std::make_shared<Enemy>("Enemy" + std::to_string(enemies.size() + 1));

	e->position.x = 2;
	e->position.y = 2;

	drawables.emplace_back(e);
	enemies.emplace_back(e);

	for (int i = 1; i < 4; i++) {
		auto sus = std::make_shared<Enemy>("Enemy" + std::to_string(enemies.size() + 1));

		sus->position.x = i * 1.3f;
		sus->position.y = i * 1.3f;

		drawables.emplace_back(sus);
		enemies.emplace_back(sus);
	}

	auto c = std::make_shared<Coin>("coin");

	c->position.y = 2;

	drawables.emplace_back(c);
	coins.emplace_back(c);

	// Create border around play area.
	b2Vec2 v[4];
	v[0].Set(-0.5f, 5.5f);
	v[1].Set(5.5f, 5.5f);
	v[2].Set(5.5f, -0.5f);
	v[3].Set(-0.5f, -0.5f);
	border.CreateLoop(v, 4);

	battleOptions = Panel({ 400, 100 }, { 200, 100 }, glm::vec3(0.3f, 0.5f, 0.3f));
	battleOptions.active = false;

	Button physical(35, 60, 60, 37, 435, 440, "ATTACK", [&] { player->physical(targetIterator->get()); });
	physical.text.scale = 0.65f;
	physical.text.font = "FinalF";
	battleOptions.addButton(physical);

	Button magic(105, 60, 60, 37, 505, 440, "MAGIC", [&] { player->magic(targetIterator->get()); });

	magic.text.scale = 0.65f;
	magic.text.font = "FinalF";
	battleOptions.addButton(magic);

	const glm::mat4 textProj = glm::ortho(-1.f, 1.f, -0.95f, 0.95f, -3.f, 3.f);

	title =			std::make_shared<Text>("Not so Final Fantasy", "FinalF", glm::vec2( -0.4f, 0.3f ), glm::vec3(0), 0.004f, textProj);
	press =			std::make_shared<Text>("Press 'G' to start!", "FinalF", glm::vec2( - 0.25f, 0 ), glm::vec3(0), 0.003f, textProj);
	pauseText =		std::make_shared<Text>("Paused!", "FinalF", glm::vec2( -0.2f, 0 ), glm::vec3(0), 0.004f, textProj);
	win =			std::make_shared<Text>("You Win!", "FinalF", glm::vec2(0), glm::vec3(0), 0.004f, textProj);
	health =		std::make_shared<Text>("HP: " + std::to_string((int)player->health), "FinalF", glm::vec2(- 0.9f, -0.7f), glm::vec3(0), 0.003f, textProj);
	mana =			std::make_shared<Text>("MA: " + std::to_string((int)player->mana), "FinalF", glm::vec2( - 0.9f, -0.84f), glm::vec3(0), 0.003f, textProj);
	gameOverTxt =	std::make_shared<Text>("Game Over!", "FinalF", glm::vec2(0, 0), glm::vec3(0), 0.004f, textProj);
	howBattle =		std::make_shared<Text>("Mash Attack or Magic to deal damage!", "FinalF", glm::vec2(-0.9f, 0.8f), glm::vec3(0), 0.003f, textProj);
	scoreTxt = std::make_shared<Text>(std::to_string(score), "FinalF", glm::vec2(0.8f, 0.8f), glm::vec3(0), 0.004f, textProj);

	howBattle->active = false;
	pauseText->active = false;
	win->active = false;
	gameOverTxt->active = false;

	ui.emplace_back(title);
	ui.emplace_back(press);
	ui.emplace_back(pauseText);
	ui.emplace_back(win);
	ui.emplace_back(health);
	ui.emplace_back(mana);
	ui.emplace_back(gameOverTxt);
	ui.emplace_back(howBattle);
	ui.emplace_back(scoreTxt);

	victorySound = "sounds/winBattle.wav";
	bgm = "sounds/bgm.wav";
	bgm.volume = 0.5f;
	bgm.looped = true;
	winSound = "sounds/win.wav";
	battle = "sounds/battle.wav";

	drawderator = drawables.begin();

	camera = Camera::fromOrtho(1, 0.95f, 3);
}

void FFScene::update() {
	if (Config::PlayMode && !inMenu && !paused && !inVictory && !gameOver) {

		if (player->health <= 0) {
			// Game over bitch
			gameOver = true;
			gameOverTxt->active = true;
		}

		player->update();
		for (auto& e : enemies)
			e->update();

		for (auto& c : coins)
			c->update();

		mana->str = "MA: " + std::to_string((int)player->mana);
		health->str = "HP: " + std::to_string((int)player->health);

		if (battleMode) {
			if ((*targetIterator)->health <= 0) {
				battleMode = false;
				healthBars.clear();

				enemies.erase(targetIterator);

				victorySound.start();
				howBattle->active = false;

				battle.stop();

				player->position = playerPosBeforeBattle;
				player->inCombat = false;
				camera.position = playerPosBeforeBattle;

				battleOptions.active = false;

				if (enemies.empty()) {
					inVictory = true;
					win->active = true;
					bgm.stop();
					victorySound.stop();
					winSound.start();
				}
			}
		} else {
			camera.position = player->position;

			if (!bgm.playing && !victorySound.isPlaying())
				bgm.resume();
		}

		checkCollision();
	}

	camera.update();
}

void FFScene::draw() {
	for (auto& o : drawables) {

		o->draw(camera.view, camera.getProjection());

		if (Config::PlayMode)
			continue;

		const auto* temp = &o->boxCollider;

		for (int32 i = 0; i < temp->m_count; i++) {
			glBegin(GL_LINES);
			glVertex3f(temp->m_vertices[i].x + o->position.x - camera.position.x, temp->m_vertices[i].y + o->position.y - camera.position.y, -2);
			glVertex3f(temp->m_vertices[(i + 1) % temp->m_count].x + o->position.x - camera.position.x, temp->m_vertices[(i + 1) % temp->m_count].y + o->position.y - camera.position.y, -2);
			glEnd();
		}

		temp = nullptr;
	}

	for (const auto& t : ui) {
		t->draw();
	}

	for (auto& h: healthBars) {
		h.draw();
	}

	if (battleMode)
		battleOptions.draw();

	if (Config::PlayMode)
		return;

	for (int32 i = 0; i < border.GetChildCount(); i++) {
		b2EdgeShape edge;
		border.GetChildEdge(&edge, i);

		glBegin(GL_LINES);
		glVertex3f(edge.m_vertex1.x - camera.position.x, edge.m_vertex1.y - camera.position.y, -2);
		glVertex3f(edge.m_vertex2.x - camera.position.x, edge.m_vertex2.y - camera.position.y, -2);
		glEnd();
	}
}

void FFScene::start() {
	inMenu = false;
	title->active = false;
	press->active = false;

	bgm.start();

	std::cout << "SEND IN DA GIGGLER!!!!!\n";
}

void FFScene::pause() {
	paused = !paused;
	pauseText->active = !pauseText->active;

	if (bgm.playing)
		bgm.pause();
	else
		bgm.resume();
}

void FFScene::checkCollision() {
	b2Transform playerTransform, borderTransform;

	borderTransform.SetIdentity();
	playerTransform.Set({player->position.x, player->position.y}, 0);

	player->blockedLeft = false;
	player->blockedTop = false;
	player->blockedRight = false;
	player->blockedBottom = false;

	for (int32 i = 0; i < border.GetChildCount(); i++) {
		if (b2TestOverlap(&player->boxCollider, 0, &border, i, playerTransform, borderTransform)) {
			switch (i) {
			case 0:
				std::cout << "Top\n";
				player->blockedTop = true;
				break;
			case 1:
				std::cout << "right\n";
				player->blockedRight = true;
				break;
			case 2:
				std::cout << "bottom\n";
				player->blockedBottom = true;
				break;
			case 3:
				std::cout << "Left\n";
				player->blockedLeft = true;
				break;
			default:
				std::cout << "HOW THE FUCK??????????????????????????????????????????????????????\n";
				break;
			}
		}
	}

	auto titerator = enemies.begin();

	for (auto& e: enemies) {
		b2Transform et;
		et.SetIdentity();
		et.Set({ e->position.x, e->position.y }, 0);

		for (int32 i = 0; i < border.GetChildCount(); i++) {
			if (b2TestOverlap(&e->boxCollider, 0, &border, i, et, borderTransform)) {
				switch (i) {
				case 0:
					std::cout << "Top\n";
					e->blockedTop = true;
					break;
				case 1:
					std::cout << "right\n";
					e->blockedRight = true;
					break;
				case 2:
					std::cout << "bottom\n";
					e->blockedBottom = true;
					break;
				case 3:
					std::cout << "Left\n";
					e->blockedLeft = true;
					break;
				default:
					std::cout << "HOW THE FUCK??????????????????????????????????????????????????????\n";
					break;
				}
			}
		}

		if (b2TestOverlap(&player->boxCollider, 0, &e->boxCollider, 0, playerTransform, et)) {
			battleMode = true;
			howBattle->active = true;

			camera.position.x = -9.5;
			camera.position.y = -9.5;

			e->position.x = -9;
			e->position.y = -9;
			e->target = player;
			e->putInBattle();

			playerPosBeforeBattle = player->position;

			player->position.x = -9.5f;
			player->position.y = -9.6f;
			player->inCombat = true;

			battleOptions.active = true;

			healthBars.emplace_back(0.5f, 0.2f, 0.3f, 0.1f, e);

			targetIterator = titerator;

			bgm.pause();
			battle.start();

			break;
		}

		++titerator;
	}

	auto chiterator = coins.begin();

	for (auto& c : coins) {
		b2Transform ct;
		ct.Set({ c->position.x, c->position.y}, 0);

		if (b2TestOverlap(&player->boxCollider, 0, &c->boxCollider, 0, playerTransform, ct)) {
			score += 1;

			scoreTxt->str = std::to_string(score);

			chiterator->get()->active = false;

			coins.erase(chiterator);

			break;
		}

		++chiterator;
	}
}

void FFScene::reset() {
	clear();

	build();
}

void FFScene::clear() {
	drawables.clear();
	ground.clear();
	enemies.clear();
	player.reset();
	ui.clear();
	border.Clear();

	winSound.stop();
	victorySound.stop();
	bgm.stop();
	battle.stop();
}

void FFScene::click(int x, int y) {
	if (!battleOptions.active)
		return;

	for (auto& b: battleOptions.buttons) {
		if (b.inside(x, y))
			b.click();
	}
}
