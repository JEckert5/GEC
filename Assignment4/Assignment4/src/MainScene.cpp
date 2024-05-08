#include "MainScene.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "Time.hpp"

void MainScene::build() {
	bgm = new Audio("sounds/bgm.wav", 0.15f);
	collect = new Audio("sounds/collect.wav", 0.15f);
	win = new Audio("sounds/win.wav", 0.15f);

	auto gt = Texture::create("textures/ground.png");
	auto ot = Texture::create("textures/obstacle.png");

	for (int i = 0; i < 10; i++) {
		drawables.emplace_back(new Object("ground" + std::to_string(i + 1), DefaultMesh::Square, {gt}));

		auto g = drawables.back();

		g->gravity = gravity;
		g->hasGravity = false;

		ground.emplace_back(g);

		g->position.x = 0.5f * i;
		g->position.y = -0.3f;
	}

	drawables.emplace_back(new Object("obstacle1", DefaultMesh::Square, { ot }));
	auto o1 = drawables.back();
	obstacles.emplace_back(o1);
	o1->position.x = 2.3f;
	o1->position.y = -0.05f;

	drawables.emplace_back(new Player);
	player = reinterpret_cast<std::shared_ptr<Player>&>(drawables.back());

	player->position.y = 0.2f;

	player->gravity = gravity;

	// drawables.emplace_back(new Coin);
	auto c = reinterpret_cast<std::shared_ptr<Coin>&>(drawables.back());

	c->name = "coin1";
	c->gravity = gravity;

	c->position.x = 0.5f;
	c->position.y = 0.4f;
	c->scale = glm::vec3(0.5f, 0.5f, 0.5f);
	c->collider = glm::vec3(0.25f, 0.25f, 0.25f);

	coins.push_back(c);

	drawables.emplace_back(new Object("goal", DefaultMesh::Square, {Texture::create("textures/gerst.png")}));

	auto g = drawables.back();

	g->position.x = 5.5f;
	g->position.y = 0.3f;

	goal = g;

	// Text is sized to be window scaled but to render in main panel, we must make very small.
	auto textProj = glm::ortho(-1.f, 1.f, -0.95f, 0.95f , -3.f, 3.f);

	text.emplace_back(new Text(std::to_string(timerVal), "JetBrainsMono", { 0.6, 0.75 }, { 0, 0, 0 }, 0.004f, textProj));

	auto s = Text(std::to_string(score), "JetBrainsMono", {-0.95, 0.75}, {0, 0, 0}, 0.004f, textProj);

	text.emplace_back(new Text(std::to_string(score), "JetBrainsMono", { -0.95, 0.75 }, { 0, 0, 0 }, 0.004f, textProj));

	menu = std::make_shared<Text>(Text("Guts Goes to Target", "JetBrainsMono", { -0.4f, 0.3 }, { 0, 0, 0 }, 0.002f, textProj));
	text.emplace_back(menu);

	pause = std::make_shared<Text>(Text("Paused!", "JetBrainsMono", { -0.2f, 0.3f }, { 0, 0, 0 }, 0.003f, textProj));
	pause->active = false;
	text.emplace_back(pause);

	victory = std::make_shared<Text>(Text("You Win!", "JetBrainsMono", { -0.2f, 0.3f }, { 0, 0, 0 }, 0.003f, textProj));
	victory->active = false;
	text.emplace_back(victory);

	camera = Camera::fromOrtho(1, 0.95f, 3);
}

void MainScene::update() {
	if (Config::PlayMode && !inMenu && !paused && !inVictory) {
		if (!bgm->isPlaying()) {
			std::cout << bgm->isPlaying();
			bgm->start();
		}

		checkCollision();
		applyGravity();

		for (auto& o : drawables) {
			switch (o->type) {
			case Object::ObjectType::OBJECT:
				o->update();
				break;
			case Object::ObjectType::SPRITE:
				dynamic_cast<Sprite*>(o.get())->update();
				break;
			case Object::ObjectType::COIN:
				dynamic_cast<Coin*>(o.get())->update();
				break;
			case Object::ObjectType::PLAYER:
				player->update();
				break;
			}
		}

		timerVal -= Time::deltaTime;

		text[0]->str = std::to_string(std::ceil(timerVal));


		const float playerLeft = player->position.x - player->collider.x / 2;

		const bool inCameraLeft = playerLeft + player->collider.x >= -cameraXBound + camera.position.x;
		const bool inCameraRight = cameraXBound + camera.position.x >= playerLeft;

		if (!inCameraRight)
			camera.position.x += player->speed * Time::deltaTime;
		else if (!inCameraLeft)
			camera.position.x -= player->speed * Time::deltaTime;
	}

	camera.update();
}

void MainScene::checkCollision() {

	// for (auto& o: )

	//// Check Ground collision, kind of doesn't work as intended but whatever.
	//for (auto i = ground.cbegin(); i != ground.cend(); ++i) {
	//	if (CheckCollision(*player, **i)) {
	//		player->grounded = true;

	//		break;
	//	}

	//	player->grounded = false;
	//}

	//// Very awful collision checker for the obstacles.
	//for (auto i = obstacles.cbegin(); i != obstacles.cend(); ++i) {
	//	if (CheckCollisionLR(*player, **i)) {
	//		player->position.x += player->speed * Time::deltaTime;
	//	}

	//	if (CheckCollisionRL(*player, **i)) {
	//		player->position.x -= player->speed * Time::deltaTime;
	//	}

	//	if (CheckCollision(*player, **i)) {
	//		player->yVel = 0;
	//	}
	//}

	//for (auto i = coins.cbegin(); i != coins.cend(); ++i) {
	//	if (CheckCollision(*player, **i)) {
	//		score += 1;
	//		text[1]->str = std::to_string(score);

	//		for (auto ci = children.begin(); ci != children.end(); ++ci) {
	//			if ((*ci)->type == Object::ObjectType::COLLECTABLE) {
	//				auto cc = reinterpret_cast<std::shared_ptr<Coin>&>(*ci);

	//				if (cc == *i) {
	//					children.erase(ci);
	//					break;
	//				}
	//			}
	//		}

	//		coins.erase(i);

	//		break;
	//	}
	//}

	//if (CheckCollision(*player, *goal)) {
	//	inVictory = true;
	//	victory->active = true;
	//	win->start();

	//	std::cout << "Goal!\n";
	//}
}

void MainScene::applyGravity() const {
	for (auto& o: drawables) {
		// std::cout << o->name << "\n";

		if (!o->hasGravity || o->type == Object::ObjectType::PLAYER)
			continue;

		o->velocity.y -= o->gravity * Time::deltaTime * Time::deltaTime;
	}
}

void MainScene::draw() {
	for (auto& o: drawables) {
		o->draw(camera.view, camera.getProjection());
	}

	for (const auto& t: text) {
		t->draw();
	}
}

void MainScene::reset() {
	drawables.clear();
	text.clear();

	timerVal = 500;

	bgm->stop();
	win->stop();
	collect->stop();

	inMenu = true;
	paused = false;
	inVictory = false;

	menu->active = true;
	pause->active = false;
	victory->active = false;

	build();
}
