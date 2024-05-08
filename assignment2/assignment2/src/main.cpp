#include <bitset>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Object.hpp"
#include <GL/freeglut.h>
#include <iostream>

#include "Audio.hpp"
#include "Char.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

constexpr int WIN_WIDTH = 600;
constexpr int WIN_HEIGHT = 600;
constexpr int WIN_X = 100;
constexpr int WIN_Y = 100;

Shader DefaultShader;
Shader CharShader;

int Window;

double Gravity = -1;
double CameraPos = 0.0;

int Timer = 500;
double YVel = 0.0;
double JumpHeight = 0.000003;
double Speed = 1;
int Score = 0;
bool Jumped = false;
bool Grounded = true;
bool Paused = false;
bool ShowAxes = true;
bool MainMenu = true;
bool GameOver = false;
bool Victory = false;

Player PlayerObj;
glm::mat4 Camera = glm::identity<glm::mat4>();
glm::mat4 Projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -5.f, 5.f);
Text PauseText; 
Text TitleText;
Text PressText;
Text TimerText; 
Text GameOverText;
Text VictoryText;
Text ScoreText;
Audio WinSound("sounds/win.wav");
Audio JumpSound("sounds/jump.wav");
Audio BGM("sounds/bgm.wav", 0.15f, true);
std::vector<Object> Ground;
std::vector<Object> Obstacles;
std::vector<Item> Collectables;
Object Goal;
Object Background;

double ElapsedTime = 0.0;
double DeltaTime = 0.0;

void ProcessSpecial() { // This version is able to detect if it is held
	if (Paused || MainMenu || Victory || GameOver) return;

	if (GetAsyncKeyState(VK_RIGHT)) {
		PlayerObj.position.x += Speed * DeltaTime;

		for (auto& o : Obstacles) {
			if (CheckCollision(PlayerObj, o)) {
				PlayerObj.position.x -= Speed * DeltaTime;
				break;
			}
		}

		return;
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		PlayerObj.position.x -= Speed * DeltaTime;

		for (auto& o : Obstacles) {
			if (CheckCollision(PlayerObj, o)) {
				PlayerObj.position.x += Speed * DeltaTime;
				break;
			}
		}

		return;
	}
}

void ProcessKeyInput(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			ShowAxes = !ShowAxes;
			break;
		case 'g':
			MainMenu = false;
			break;
		case 'p':
			Paused = !Paused;
			if (BGM.paused) {
				BGM.resume();
			} else {
				BGM.pause();
			}
			break;
		case 27: // Esc
			exit(0);
		case 32: // Space
			if (Grounded && !Paused && !MainMenu && !GameOver && !Victory) {
				YVel = sqrtf(JumpHeight * -Gravity);
				Jumped = true;
				JumpSound.start();
			}
			break;
		default:
			break;
	}
}

void Init(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(WIN_X, WIN_Y);
	//glutInitContextVersion(4, 6); // Breaks glBegin() and glEnd(). idk why.
	glutInitContextProfile(GLUT_CORE_PROFILE);

	Window = glutCreateWindow("Jens Eckert jeckert5");

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Could not initialize glew!\n";
		exit(EXIT_FAILURE);
	}
	
	std::cout << glGetString(GL_VERSION) << "\n";

	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

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

		Char::Chars.insert(std::pair<char, Char>(c, ch));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void DrawAxes() {
	if (!ShowAxes)
		return;

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
}

void DisplayFunc() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	const double et = static_cast<double>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;
	DeltaTime = et - ElapsedTime;
	ElapsedTime = et;

	if (!Paused && !MainMenu && !GameOver && !Victory) {

		ProcessSpecial();

		if (Timer == 0) {
			GameOver = true;
		}

		if (CheckCollision(PlayerObj, Goal)) {
			Victory = true;
			WinSound.start();
			goto VictoryLabel;
		}

		for (auto& g : Ground) {
			if (CheckCollision(PlayerObj, g)) {
				Grounded = true;
				break;
			}

			Grounded = false;
		}

		for (auto itr = Collectables.begin(); itr != Collectables.end(); ++itr) {
			if (CheckCollision(PlayerObj, *itr)) {
				Score += 1;
				Collectables.erase(itr);
				break;
			}
		}

		const auto grav = Gravity * DeltaTime * DeltaTime;

		if (Grounded && !Jumped) {
			YVel = 0;
		} else {
			YVel += grav;
		}

		if (Jumped) Jumped = false;

		PlayerObj.position.y += YVel;

		for (auto& o : Obstacles) {
			if (CheckCollision(PlayerObj, o)) {
				PlayerObj.position.y -= YVel;
				YVel = 0;
				Grounded = true;
				break;
			}
		}

		CameraPos = -PlayerObj.position.x;
		Background.position.x = PlayerObj.position.x;

		Camera = glm::identity<glm::mat4>();
		Camera = glm::translate(Camera, glm::vec3(CameraPos, 0, 0));

		PlayerObj.update(DeltaTime, ElapsedTime);

		for (auto& c : Collectables)
			c.update(DeltaTime, ElapsedTime);

		if (PlayerObj.position.y <= -0.5f) {
			GameOver = true;
		}
	}

	TimerText.text = std::to_string(Timer);
	ScoreText.text = "Score: " + std::to_string(Score);

	VictoryLabel:
	Background.draw(Camera, Projection);

	PlayerObj.draw(Camera, Projection);

	for (auto& g : Ground)
		g.draw(Camera, Projection);

	for (auto& o : Obstacles)
		o.draw(Camera, Projection);

	for (auto& c : Collectables)
		c.draw(Camera, Projection);

	Goal.draw(Camera, Projection);

	DrawAxes();

	TimerText.draw();
	ScoreText.draw();

	if (Paused)
		PauseText.draw();

	if (MainMenu) {
		TitleText.draw();
		PressText.draw();
	}

	if (GameOver)
		GameOverText.draw();

	if (Victory)
		VictoryText.draw();

	glFlush();
	glutSwapBuffers();

	// Sleep(16);
}

void UpdateTimer(int value) {
	if (!MainMenu && !Paused && !Victory && !GameOver)
		Timer -= 1;

	glutPostRedisplay();
	glutTimerFunc(1000, UpdateTimer, value);
}

int main(int argc, char** argv) {
	Init(argc, argv);

	glutKeyboardFunc(ProcessKeyInput);
	glutTimerFunc(0, UpdateTimer, 0);

	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(DisplayFunc);

	DefaultShader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	CharShader = Shader("shaders/charvertex.glsl", "shaders/charfragment.glsl");

	Texture floor("textures/floor.png");
	Texture obstacle("textures/obstacle.png");
	Texture goal("textures/goal.jpg");
	Texture item("textures/coinSheet.png");
	Texture bg("textures/bg.png");

	PlayerObj = Player(&DefaultShader);
	Background = Object(Square, &DefaultShader, bg);
	Background.scale = glm::vec3(8, 8, 1);

	Goal = Object(Square, &DefaultShader, goal);

	Goal.position.x = 10.5f;

	auto goColor = glm::vec3(1.0f, 0.2f, 0.3f);
	auto winColor = glm::vec3(0.4, 1.0f, 0.2f);
	auto baseColor = glm::vec3(0.5f, 0.2f, 0.95f);

	PauseText = Text("Paused", { 230, 300 }, baseColor, &CharShader);
	TitleText = Text("Have You Cena The Floor?", { 55, 150 }, baseColor, &CharShader, 0.75f);
	PressText = Text("Press 'g' to start!", { 165, 110 }, baseColor, &CharShader, 0.5f);
	TimerText = Text("Score: " + std::to_string(Timer), { 20, 530 }, baseColor, &CharShader);
	GameOverText = Text("Game Over!", { 230, 300 }, goColor, &CharShader);
	VictoryText = Text("You Win!", { 230, 300 }, winColor, &CharShader);
	ScoreText = Text(std::to_string(Score), { 350, 530 }, baseColor, &CharShader);

	constexpr auto collider = glm::vec3(0.5f);
	constexpr auto gscale = glm::vec3(1, 1, 1);
	constexpr auto gypos = -0.6f;
	constexpr auto oscale = glm::vec3(1, 1, 1);

	Goal.collider = collider;

	PlayerObj.collider = glm::vec3(0.4f, 0.4f, 1);
	PlayerObj.scale = glm::vec3(2, 2, 1);

	float f = 0;

	for (int i = 0; i < 20; i++) {
		Object g(Square, &DefaultShader, floor);

		g.position.y = gypos;
		g.position.x = f;
		g.scale = gscale;
		g.collider = collider;

		f += 0.5f;

		Ground.emplace_back(g);
	}

	f = 5;

	for (int i = 0; i < 2; i++) {
		Object o(Square, &DefaultShader, obstacle);

		o.position.y = -0.2f;
		o.position.x = f;
		o.scale = gscale;
		o.collider = collider;

		f += 3;

		Obstacles.emplace_back(o);
	}

	Item i1(&DefaultShader, item);
	Item i2(&DefaultShader, item);
	Item i3(&DefaultShader, item);

	i1.position.y = 1;

	i2.position.x = 4;
	i2.position.y = 0.2f;

	i3.position.x = 7;

	Collectables.emplace_back(i1);
	Collectables.emplace_back(i2);
	Collectables.emplace_back(i3);

	BGM.start();

	glutMainLoop();

	return 0;
}