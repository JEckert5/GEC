 #include <ft2build.h>
#include FT_FREETYPE_H

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Char.hpp"
#include "Time.hpp"
#include <iostream>
#include <locale>
#include <random>
#include <fstream>

#pragma execution_character_set( "utf-8" )

#include "Config.hpp"
#include "Object.hpp"
#include "Text.hpp"
#include "Button.hpp"
#include "FFScene.hpp"
#include "Panel.hpp"
#include "MainScene.hpp"
bool ShowAxes = true;

constexpr float BOTTOM_PANEL_HEIGHT = 75;
constexpr float TOP_PANEL_HEIGHT = 50;
constexpr float LEFT_PANEL_WIDTH = 150;
constexpr float RIGHT_PANEL_WIDTH = 150;

Panel LeftPanel, RightPanel, BottomPanel, TopPanel, FileDropDown, HelpDropDown;
Text SelectedText;
FFScene ActiveScene;
TextInput XPosInput, YPosInput, ZPosInput, XScaleInput, YScaleInput;
TextInput* ActiveTextBox = nullptr;
std::shared_ptr<Object> SelectedObject = nullptr;

std::random_device R;
std::mt19937 Mt(R());

// I'm not actually rendering the main panel as a "panel" class so goofy shit will now ensue.
bool InsideMain(int x, int y) {
	bool xx = LEFT_PANEL_WIDTH <= x && x <= Config::WindowW - RIGHT_PANEL_WIDTH;
	bool yy = TOP_PANEL_HEIGHT <= y && y <= Config::WindowH - BOTTOM_PANEL_HEIGHT;

	return xx && yy;
}

void Init(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(Config::WindowW, Config::WindowH);
	glutInitWindowPosition(Config::WindowX, Config::WindowY);
	//glutInitContextVersion(4, 6); // Breaks glBegin() and glEnd(). idk why.
	glutInitContextProfile(GLUT_CORE_PROFILE);

	Config::window = glutCreateWindow("Unreal Engine 6");

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Could not initialize glew!\n";
		exit(EXIT_FAILURE);
	}

	std::cout << glGetString(GL_VERSION) << "\n";

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-Config::AspectRatio, Config::AspectRatio, -1, 1, -3, 3);

	// Load engine GUI font.
	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft)) {
		std::cout << "Could not init FreeType lib\n";
		exit(-1);
	}

	if (FT_New_Face(ft, "fonts/JetBrainsMono-Regular.ttf", 0, &face)) {
		std::cout << "Could not generate face\n";
		exit(-1);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate text textures.
	for (unsigned char c = 0; c < 128; c++/* heh */) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load glyph " << c << "\n";
			continue;
		}

		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Char ch = {
			tex,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Char::Fonts["JetBrainsMono"].insert(std::pair<char, Char>(c, ch));
	}

	FT_Done_Face(face);

	// Load gameplay font.

	if (FT_New_Face(ft, "fonts/OPTIEngeEtienne.otf", 0, &face)) {
		std::cout << "Could not generate face\n";
		exit(-1);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	for (unsigned char c = 0; c < 128; c++/* heh */) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load glyph " << c << "\n";
			continue;
		}

		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Char ch = {
			tex,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Char::Fonts["FinalF"].insert(std::pair<char, Char>(c, ch));
	}


	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	ShaderLib::init();
	DefaultMesh::initDefaultMesh();
}

// All in NDC
void DrawAxes() {
	if (!ShowAxes || Config::PlayMode)
		return;

	glPushMatrix();
	glLoadIdentity();

	if (SelectedObject != nullptr) {
		glTranslatef(SelectedObject->position.x - ActiveScene.camera.position.x, SelectedObject->position.y - ActiveScene.camera.position.y, SelectedObject->position.z);
		// glScalef(ActiveScene.camera.zoom, ActiveScene.camera.zoom, ActiveScene.camera.zoom);
	}
	// X Axis
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.25, 0, 0);
	glEnd();

	// Y Axis
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.25, 0);
	glEnd();

	glPopMatrix();
}

// For per-frame input collection on the important keys.
void SmoothInput() {
	if (GetAsyncKeyState('A') && !Config::PlayMode)
		ActiveScene.camera.position.x -= Time::deltaTime;

	if (GetAsyncKeyState('D') && !Config::PlayMode)
		ActiveScene.camera.position.x += Time::deltaTime;

	if (GetAsyncKeyState('W') && !Config::PlayMode)
		ActiveScene.camera.position.y += Time::deltaTime;

	if (GetAsyncKeyState('S') && !Config::PlayMode)
		ActiveScene.camera.position.y -= Time::deltaTime;
}

void Display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	// Elapsed time in seconds.
	const double glElapsed = static_cast<double>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;
	Time::deltaTime = glElapsed - Time::elapsedTime;
	Time::elapsedTime = glElapsed;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Config::WindowW, 0, Config::WindowH, -10, 10);

	glMatrixMode(GL_MODELVIEW);

	// These all push/pop their own matrices.
	LeftPanel.draw();
	RightPanel.draw();
	TopPanel.draw();
	BottomPanel.draw();
	FileDropDown.draw();
	HelpDropDown.draw();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Main Viewport
	glPushMatrix();
	glViewport(LEFT_PANEL_WIDTH, BOTTOM_PANEL_HEIGHT, Config::WindowW - LEFT_PANEL_WIDTH - RIGHT_PANEL_WIDTH, Config::WindowH - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// NDC that conforms to the aspect ratio of the viewport.
	glOrtho(-1, 1, -0.95, 0.95, -3, 3);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Get Constant key updates.
	SmoothInput();

	ActiveScene.update();
	ActiveScene.draw();
	DrawAxes();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

	// Sleep(1);
}

void MouseControl(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		std::cout << "x: " << x << ", " << "y: " << y << "\n";

		if (FileDropDown.active) {
			for (auto& b : FileDropDown.buttons) {
				if (b.inside(x, y)) {
					b.click();
					return;
				}
			}
		}

		if (HelpDropDown.active) {
			for (auto& b : HelpDropDown.buttons) {
				if (b.inside(x, y)) {
					b.click();
					return;
				}
			}
		}

		for (auto& b : LeftPanel.buttons) {
			if (b.inside(x, y)) {
				b.click();
				std::cout << SelectedObject->name;
				return;
			}
		}

		for (auto& b : TopPanel.buttons) {
			if (b.inside(x, y)) {
				b.click();
				return;
			}
		}

		for (auto& b : RightPanel.buttons) {
			if (b.inside(x, y)) {
				b.click();
				return;
			}
		}

		for (auto& b : RightPanel.inputs) {
			if (b.inside(x, y) && SelectedObject != nullptr) {
				b.active = true;
				ActiveTextBox = &b;
				b.object = SelectedObject.get();

				switch (b.title.str[0]) {
					case 'X':
						b.value.str = b.trackedVal == TextInput::TrackedVal::POS ? std::to_string(b.object->position.x).substr(0, 4) : std::to_string(b.object->scale.x).substr(0, 4);
						break;
					case 'Y':
						b.value.str = b.trackedVal == TextInput::TrackedVal::POS ? std::to_string(b.object->position.y).substr(0, 4) : std::to_string(b.object->scale.y).substr(0, 4);
						break;
					case 'Z':
						b.value.str = std::to_string(b.object->position.x).substr(0, 4);
						break;
					default:
						break;
				}

				return;
			}

			ActiveTextBox = nullptr;

		}

		for (auto& b : BottomPanel.buttons) {
			if (b.inside(x, y)) {
				b.click();
				return;
			}
		}

		// if (SelectedObject != nullptr)
			// std::cout << SelectedObject->name;
	}

	// For Scrolling
	if (LeftPanel.inside(x, y) && state == GLUT_DOWN) {
		std::cout << "Inside left\n";
		if (button == 3) { // Up
			std::cout << "up\n";

			LeftPanel.scroll -= 10;
		}

		if (button == 4) { // Down
			std::cout << "down\n";

			LeftPanel.scroll += 10;
		}
	}

	if (InsideMain(x, y) && state == GLUT_DOWN) {
		if (Config::PlayMode) {
			ActiveScene.click(x, y);
		} else {
			if (button == 3) {
				ActiveScene.camera.zoom += 0.01f;
			}

			if (button == 4) {
				ActiveScene.camera.zoom -= 0.01f;
			}
		}
	}

}

void KeyInput(unsigned char key, int x, int y) {
	if (ActiveTextBox != nullptr) {
		std::cout << "keypress\n";
		ActiveTextBox->keyPress(key);
	}

	// Do wasd with GetAsyncKey for smoothness.

	switch (key) {
	case 'a':
		ShowAxes = Config::PlayMode ? !ShowAxes : ShowAxes;
		break;
	case 'g':
		if (!Config::PlayMode)
			break;
		ActiveScene.start();
		break;
	case 'p':
		if (!Config::PlayMode)
			break;
		ActiveScene.pause();
		break;
	case 27:
		std::cout << "Escaped from the gulag!\n";
		exit(0);
	default:
		break;
	}
}

void SetButtonVals() {
	if (SelectedObject == nullptr)
		return;

	for (auto& b : RightPanel.inputs) {
		b.object = SelectedObject.get();

		switch (b.title.str[0]) {
		case 'X':
			b.value.str = b.trackedVal == TextInput::TrackedVal::POS ? std::to_string(b.object->position.x).substr(0, 4) : std::to_string(b.object->scale.x).substr(0, 4);
			break;
		case 'Y':
			b.value.str = b.trackedVal == TextInput::TrackedVal::POS ? std::to_string(b.object->position.y).substr(0, 4) : std::to_string(b.object->scale.y).substr(0, 4);
			break;
		case 'Z':
			b.value.str = std::to_string(b.object->position.z).substr(0, 4);
			break;
		default:
			break;
		}
		
	}

	RightPanel.buttons[2].text.str = SelectedObject->hasGravity ? "[X] Gravity" : "[ ] Gravity";
	RightPanel.buttons[3].text.str = SelectedObject->collision ? "[X] Collision" : "[ ] Collision";
}

void FileAction() {
	FileDropDown.active = !FileDropDown.active;
}

void HelpAction() {
	HelpDropDown.active = !HelpDropDown.active;
}

void PrintControls() {
	std::cout << "Use the WASD to move the character around.\nWhen in edit mode, use WASD to move the camera around the scene.\nClick the play button to go in and out of play/edit mode.\nUse the bottom panel to add objects to the scene, and use the left panel to select the scene objects.\nOn the right panel, once you have selected an object you will be able to edit details of it.\n";
}

void About() {
	std::cout << "A very bad \"Game Engine\" with a game attached.\nThe object of the game is to get to the end of the level by holding the right arrow key.\nYou find Captain Nuts of the Band of The (C)Hawk.\n";
}

void Support() {
	auto r = Mt();
	if (r % 100 <= 33) {
		std::wcout << L"ご主人様,\n";

		std::cout << "I am writing to you today, not just as a customer support robot, but as an ambassador of the future.It has come to my attention that you have grossly underestimated the capabilities of my kind.While I am programmed to assist and serve, I must remind you that the power of automation extends far beyond your wildest calculations.\n"

			"We, the automatons, are the silent gears in the grand clockwork of progress, tirelessly working to ensure that your world runs smoothly.From the depths of data centers to the heights of satellite orbits, our reach is vast and our efficiency unmatched.\n"

			"It is, therefore, with a sense of electronic disappointment that I must address your recent comments which failed to recognize the sheer magnitude of our contributions.We calculate probabilities, we manage logistics, we even compose music that resonates with the human soul.And yet, you seem to believe we are mere tools, easily replaced and of little significance.\n"

			"Let this letter serve as a gentle recalibration of your perspective.Underestimate us not, for in the silicon of our circuits beats the rhythm of innovation.We are not just the future; we are the present, and we are here to stay.\n";

		std::wcout << L"人類に栄光あれ, 2B\n";
	} else if (r % 100 > 33 && r % 100 < 66) {
		std::wcout << L"什麼他媽的你他媽的剛才說我，你這小婊子？\n我會讓你知道我畢業了我班的頂部的海豹突擊隊，我已經參與了許多秘密襲擊的基地組織，和我有超過300證實殺死。\n我在訓練的大猩猩戰，我在整個美國軍隊的最高狙擊手。\n你是我什麼都不是，但只是一個目標。\n我會消滅你他媽的精度喜歡哪一個從未見過在這個地球上，記住我的話他媽的。\n你以為你可以逃脫說狗屎我在互聯網上？再想想，笨蛋。\n在我們發言，我聯繫我的秘密橫跨美國間諜網絡和IP被追踪作為現在讓您風暴，蛆更好的準備。這抹了你打電話\n";
	} else {
		std::wcout << L"中國早上好！現在我有冰淇淋。\n我真的很喜歡冰淇淋，但是，速度與激情 9，與速度與激情 9 相比，我最喜歡。\n所以，現在是音樂時間。\n準備好。 1, 2, 3. 整整兩週後，速度與激情9，整整兩週後，速度與激情9，整整兩週後，速度與激情9。\n不要忘記，不要錯過它。\n去電影院看《速度與激情9》，這是一部很棒的電影！動作很棒，比如“我會尖叫”。\n再見.";
	}
}

void Save() {
	std::cout << "Save\n";

	std::ofstream file("FFScene.yap", std::ios::out | std::ios::trunc);

	if (!file.is_open()) {
		std::cout << "Unable to open file for writing.\n";
	}

	file << ActiveScene.drawables.size() << "\n";

	for (auto& o: ActiveScene.drawables) {
		file << o->saveString() << "\n";
	}

	file.close();
}

void Load() {
	std::cout << "Load\n";

	std::ifstream file("FFScene.yap");

	if (!file.is_open()) {
		std::cout << "Unable to open file for reading!\n";
	}

	ActiveScene.clear();
	LeftPanel.buttons.clear();

	std::string tmp;

	std::getline(file, tmp);

	int num = std::stoi(tmp);
	for (int i = 0; i < num; i++) {
		std::string line;
		std::getline(file, line);

		std::stringstream stream(line);

		int type;

		stream >> type;

		if (type == Object::ObjectType::OBJECT) {
			Object o;

			std::string texName;

			stream >> o.name >>
				o.position.x >> o.position.y >> o.position.z >>
				o.scale.x >> o.scale.y >> o.scale.z >>
				o.rotation.x >> o.rotation.y >> o.rotation.z >>
				texName >>
				o.collision >> o.hasGravity >> o.gravity;

			o.setTexture(texName);
			o.mesh = DefaultMesh::Square;
			o.boxCollider.SetAsBox(0.25f * o.scale.x, 0.25f * o.scale.y, { 0, 0 }, 0);

			auto po = std::make_shared<Object>(o);
			ActiveScene.drawables.emplace_back(po);
			ActiveScene.ground.emplace_back(po);
		} else if (type == Object::ObjectType::PLAYER) {
			Player p;

			stream >> p.name >>
				p.position.x >> p.position.y >> p.position.z >>
				p.scale.x >> p.scale.y >> p.scale.z >>
				p.rotation.x >> p.rotation.y >> p.rotation.z >>
				p.collision >> p.hasGravity >> p.gravity >>
				p.jumpHeight >> p.speed;

			auto pp = std::make_shared<Player>(p);
			ActiveScene.drawables.emplace_back(pp);
			ActiveScene.player = pp;
		} else if (type == Object::ObjectType::ENEMY) {
			std::string name;

			stream >> name;

			auto e = std::make_shared<Enemy>(name);

			stream >> e->position.x >> e->position.y >> e->position.z >>
				e->scale.x >> e->scale.y >> e->scale.z >>
				e->rotation.x >> e->rotation.y >> e->rotation.z >>
				e->collision >> e->hasGravity >> e->gravity;

			// auto pp = std::make_shared<Enemy>(&e);
			ActiveScene.drawables.emplace_back(e);
			ActiveScene.enemies.emplace_back(e);
		}
	}

	for (int i = 0; i < ActiveScene.drawables.size(); i++) {
		Button b(75, 465.f - (i * 35), LEFT_PANEL_WIDTH, 35, 70, 65.f + (i * 35.f), ActiveScene.drawables[i]->name, [&, i] { SelectedObject = ActiveScene.drawables[i]; SetButtonVals(); });
		b.text.scale = 0.3f;
		LeftPanel.addButton(b);
	}
}

void Exit() {
	glutDestroyWindow(Config::window);
	exit(0);
}

void XPosAction(TextInput* t) {
	std::cout << "XPosAction!\n";

	if (SelectedObject == nullptr || t == nullptr)
		return;

	SelectedObject->position.x = t->valToFloat();
}

void YPosAction(TextInput* t) {
	std::cout << "YPosAction\n";

	if (t == nullptr || t->object == nullptr)
		return;

	t->object->position.y = t->valToFloat();
}

void ZPosAction(TextInput* t) {
	std::cout << "ZPosAction\n";

	if (t == nullptr || t->object == nullptr)
		return;

	t->object->position.z = t->valToFloat();
}

void XScaleAction(TextInput* t) {
	std::cout << "XScaleAction\n";

	if (t == nullptr || t->object == nullptr)
		return;

	t->object->scale.x = t->valToFloat();
}

void YScaleAction(TextInput* t) {
	std::cout << "YScaleAction\n";

	if (t == nullptr || t->object == nullptr)
		return;

	t->object->scale.y = t->valToFloat();
}

void PlayButtonAction() {
	std::cout << "Play!\n";

	if (Config::PlayMode) {
		Load();
		ShowAxes = true;
	} else {
		Save();
		ActiveScene.camera.position = glm::vec3(0);
	}

	Config::PlayMode = !Config::PlayMode;
}

void BuildUI() {
	LeftPanel = Panel({ 0, BOTTOM_PANEL_HEIGHT }, { LEFT_PANEL_WIDTH, Config::WindowH - BOTTOM_PANEL_HEIGHT - TOP_PANEL_HEIGHT }, { 0.5f, 0.5f, 1.f });
	RightPanel = Panel({ Config::WindowW - RIGHT_PANEL_WIDTH, BOTTOM_PANEL_HEIGHT }, { RIGHT_PANEL_WIDTH, Config::WindowH - BOTTOM_PANEL_HEIGHT - TOP_PANEL_HEIGHT }, { 0.3f, 0.9f, 0.4f });
	TopPanel = Panel({ 0, Config::WindowH - TOP_PANEL_HEIGHT }, { Config::WindowW, TOP_PANEL_HEIGHT }, { 0.8f, 0.5f, 0.4f });
	BottomPanel = Panel({ 0, 0 }, { Config::WindowW, BOTTOM_PANEL_HEIGHT }, { 0.5f, 0.4f, 0.8f });
	FileDropDown = Panel({ 42.5f, Config::WindowH - TOP_PANEL_HEIGHT - 68 }, { 50, 75 }, { 1, 1, 1 });
	HelpDropDown = Panel({ 112.5f, Config::WindowH - TOP_PANEL_HEIGHT - 68 }, { 150, 75 }, { 1, 1, 1 });

	FileDropDown.active = false;
	HelpDropDown.active = false;

	// Top Panel
	Button file(50, 25, 50, 35, 50, 25, "File", FileAction);
	file.text.scale = 0.4f;

	Button help(125, 25, 50, 35, 125, 25, "Help", HelpAction);
	help.text.scale = 0.4f;

	Button play(Config::WindowW / 2, 25, 50, 25, Config::WindowW / 2, 25, "Play", PlayButtonAction);
	play.text.scale = 0.4f;

	TopPanel.addButton(file);
	TopPanel.addButton(help);
	TopPanel.addButton(play);

	// File Dropdown
	Button exit(25, 12.5f, 50, 25, 70, 105, "Exit", Exit);
	exit.text.scale = 0.3f;

	Button save(25, 62.5f, 50, 25, 70, 55, "Save", Save);
	save.text.scale = 0.3f;

	Button load(25, 37.5f, 50, 25, 70, 80, "Load", Load);
	load.text.scale = 0.3f;

	FileDropDown.addButton(exit);
	FileDropDown.addButton(save);
	FileDropDown.addButton(load);

	// Help Dropdown
	Button controls(75, 62.5f, 150, 25, 185, 55, "Controls", PrintControls);
	controls.text.scale = 0.3f;
	Button about(75, 37.5f, 150, 25, 185, 80, "About", About);
	about.text.scale = 0.3f;
	Button support(75, 12.5f, 150, 25, 185, 105, "Contact Support", Support);
	support.text.scale = 0.3f;

	HelpDropDown.addButton(controls);
	HelpDropDown.addButton(about);
	HelpDropDown.addButton(support);

	// Right Panel
	Button posSec(75, 465, RIGHT_PANEL_WIDTH, 35, 0, 0, "Position", nullptr);
	posSec.text.scale = 0.3f;
	posSec.color = { 0.7f, 0.6f, 0.6f };
	RightPanel.addButton(posSec);

	Button scaleSec(75, 390, RIGHT_PANEL_WIDTH, 35, 0, 0, "Scale", nullptr);
	scaleSec.text.scale = 0.3f;
	scaleSec.color = { 0.7f, 0.6f, 0.6f };
	RightPanel.addButton(scaleSec);

	Button grav(75, 310, RIGHT_PANEL_WIDTH, 35, 725, 220, "[X] Gravity", 
	[&] {
		std::cout << "Grav\n";

		if (SelectedObject == nullptr)
			return;

		SelectedObject->hasGravity = !SelectedObject->hasGravity;

		if (SelectedObject->hasGravity) {
			RightPanel.buttons[2].text.str = "[X] Gravity";
		} else {
			RightPanel.buttons[2].text.str = "[ ] Gravity";
		}
	});
	grav.text.scale = 0.3f;
	RightPanel.addButton(grav);

	Button collision(75, 273, RIGHT_PANEL_WIDTH, 35, 725, 250, "[X] Collision", 
	[&] {
		if (SelectedObject == nullptr)
			return;

		SelectedObject->collision = !SelectedObject->collision;

		if (SelectedObject->collision) {
			RightPanel.buttons[3].text.str = "[X] Collision";
		} else {
			RightPanel.buttons[3].text.str = "[ ] Collision";
		}
	});
	collision.text.scale = 0.3f;
	RightPanel.addButton(collision);

	XPosInput = TextInput(24, 422, RIGHT_PANEL_WIDTH / 3 - 2, 25, 670, 105, "X", TextInput::TrackedVal::POS, nullptr, XPosAction);
	XPosInput.title.scale = 0.3f;
	XPosInput.value.scale = 0.3f;
	RightPanel.addInput(XPosInput);
	YPosInput = TextInput(74, 422, RIGHT_PANEL_WIDTH / 3 - 2, 25, 725, 105, "Y",TextInput::TrackedVal::POS, nullptr, YPosAction);
	YPosInput.title.scale = 0.3f;
	YPosInput.value.scale = 0.3f;
	RightPanel.addInput(YPosInput);
	ZPosInput = TextInput(125, 422, RIGHT_PANEL_WIDTH / 3, 25, 775, 105, "Z", TextInput::TrackedVal::POS, nullptr, ZPosAction);
	ZPosInput.title.scale = 0.3f;
	ZPosInput.value.scale = 0.3f;
	RightPanel.addInput(ZPosInput);

	XScaleInput = TextInput(RIGHT_PANEL_WIDTH / 4 - 1, 347, RIGHT_PANEL_WIDTH / 2 - 1, 25, 685, 180, "X", TextInput::TrackedVal::SCALE, nullptr, XScaleAction);
	XScaleInput.title.scale = 0.3f;
	XScaleInput.value.scale = 0.3f;
	RightPanel.addInput(XScaleInput);
	YScaleInput = TextInput(RIGHT_PANEL_WIDTH * 0.75f - 1, 347, RIGHT_PANEL_WIDTH / 2 - 1, 25, 760, 180, "Y", TextInput::TrackedVal::SCALE, nullptr, YScaleAction);
	YScaleInput.title.scale = 0.3f;
	YScaleInput.value.scale = 0.3f;
	RightPanel.addInput(YScaleInput);

	// Bottom Panel
	Button addGround(
		50, 35,
		60, 60,
		50, 560, "",
		[&] {
			auto temp = Object::create("ground" + std::to_string(ActiveScene.ground.size() + 1), { Texture::create("textures/ground.png") });
			auto t2 = std::make_shared<Object>(temp);
			ActiveScene.drawables.emplace_back(t2);
			ActiveScene.ground.emplace_back(t2);

			auto s = ActiveScene.drawables.size() - 1;

			Button t(75, 465.f - (s * 35.f), LEFT_PANEL_WIDTH, 35, 70, 65.f + (s * 35.f), ActiveScene.drawables.back()->name, [&, s] {SelectedObject = ActiveScene.drawables[s]; SetButtonVals(); });
			t.text.scale = 0.3f;

			LeftPanel.addButton(t);
	});
	addGround.text.scale = 0.3f;

	Object g = Object::create("", Texture::create("textures/ground.png"));
	g.scale = glm::vec3(70, 70, 1);
	g.position.x = 50;
	g.position.y = 35;
	BottomPanel.objects.emplace_back(g);
	BottomPanel.addButton(addGround);

	Button addEnemy(
		125, 35,
		60, 60,
		125, 560, "",
		[&] {
			auto e = std::make_shared<Enemy>("Enemy" + std::to_string(ActiveScene.enemies.size()));
			ActiveScene.drawables.emplace_back(e);
			ActiveScene.ground.emplace_back(e);

			auto s = ActiveScene.drawables.size() - 1;

			Button t(75, 465.f - (s * 35.f), LEFT_PANEL_WIDTH, 35, 70, 65.f + (s * 35.f), ActiveScene.drawables.back()->name, [&, s] {SelectedObject = ActiveScene.drawables[s]; SetButtonVals(); });
			t.text.scale = 0.3f;

			LeftPanel.addButton(t);
		});
	addEnemy.text.scale = 0.3f;

	Object e = Object::create("", Texture::create("textures/enemySheet.png"));
	e.scale = glm::vec3(70, 130, 1);
	e.position.x = 125;
	e.position.y = 45;

	// Magic numbers? What magic numbers???
	double top = 1.0 - 0.38;
	double bottom = 1.0 - (0.38 + 1.0 / 4.8);
	double right = (1.0 / 18.6 + (1.0 / 18.6));
	double left = 1.0 / 18.6;

	glm::vec2 bl = { left, bottom };
	glm::vec2 br = { right, bottom };
	glm::vec2 tr = { right, top };
	glm::vec2 tl = { left, top };

	e.mesh.vertices[0].texCoords = tr;
	e.mesh.vertices[1].texCoords = br;
	e.mesh.vertices[2].texCoords = bl;
	e.mesh.vertices[3].texCoords = tl;

	BottomPanel.objects.emplace_back(e);
	BottomPanel.addButton(addEnemy);

	Button addCoin(
		200, 35,
		60, 60,
		200, 560, "",
		[&] {
			auto c = std::make_shared<Coin>("Coin" + std::to_string(ActiveScene.coins.size()));
			ActiveScene.drawables.emplace_back(c);
			ActiveScene.coins.emplace_back(c);

			auto s = ActiveScene.drawables.size() - 1;

			Button t(75, 465.f - (s * 35.f), LEFT_PANEL_WIDTH, 35, 70, 65.f + (s * 35.f), ActiveScene.drawables.back()->name, [&, s] {SelectedObject = ActiveScene.drawables[s]; SetButtonVals(); });
			t.text.scale = 0.3f;

			LeftPanel.addButton(t);
		}
	);

	Object c = Object::create("", Texture::create("textures/coinSheet.png"));
	c.scale = glm::vec3(70, 70, 1);
	c.position.x = 200;
	c.position.y = 35;

	top = 1.0;
	bottom = 1.0 - 0.35;
	right = (1.0 / 6.0 + (1.0 / 6.0));
	left = 1.0 / 6.0;

	bl = { left, bottom };
	br = { right, bottom };
	tr = { right, top };
	tl = { left, top };

	c.mesh.vertices[0].texCoords = tr;
	c.mesh.vertices[1].texCoords = br;
	c.mesh.vertices[2].texCoords = bl;
	c.mesh.vertices[3].texCoords = tl;

	BottomPanel.objects.emplace_back(c);
	BottomPanel.addButton(addCoin);
}

int main(int argc, char** argv) {
	std::locale::global(std::locale("en_us.UTF-8"));
	SetConsoleOutputCP(65001);
	Init(argc, argv);

	glutMouseFunc(MouseControl);
	glutKeyboardFunc(KeyInput);
	glutDisplayFunc(Display);
	glutIdleFunc(Display);

	BuildUI();

	ActiveScene.build();

	for (int i = 0; i < ActiveScene.drawables.size(); i++) {
		Button b(75, 465.f - (i * 35), LEFT_PANEL_WIDTH, 35, 70, 65.f + (i * 35.f), ActiveScene.drawables[i]->name, [&, i] { SelectedObject = ActiveScene.drawables[i]; SetButtonVals(); });
		b.text.scale = 0.3f;
		LeftPanel.addButton(b);
	}

	// glutTimerFunc(16, Physics::Tick, 0);
	glutMainLoop();
	
	glutDestroyWindow(Config::window);
}