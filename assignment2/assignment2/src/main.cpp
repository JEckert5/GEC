#include "Scene.hpp"
#include "Audio.hpp"
#include <GL/freeglut.h>
#include <iostream>

#include "Player.hpp"

constexpr int WIN_WIDTH = 600;
constexpr int WIN_HEIGHT = 600;
constexpr int WIN_X = 100;
constexpr int WIN_Y = 100;

static int Window;

Scene* MainScene;

static int ElapsedTime = 0.f;
static int DeltaTime = 0.f;

void Init(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(WIN_X, WIN_Y);
	glutInitContextVersion(4, 6);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	Window = glutCreateWindow("Jens Eckert jeckert5");

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Could not initialize glew!\n";
		exit(EXIT_FAILURE);
	}

	std::cout << glGetString(GL_VERSION);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void DisplayFunc() {
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	int et = glutGet(GLUT_ELAPSED_TIME);
	DeltaTime = et - ElapsedTime;
	ElapsedTime = et;

	MainScene->update(DeltaTime, ElapsedTime);
	MainScene->draw();

	glFlush();
	glutSwapBuffers();
}

void ProcessKeyInput(unsigned char key, int x, int y) {
	
}

void ProcessSpecialInput(int key, int x, int y) {
	
}

void ProcessMouseInput(int button, int state, int x, int y) {
	
}

int main(int argc, char** argv) {
	Init(argc, argv);

	glutKeyboardFunc(ProcessKeyInput);
	glutSpecialFunc(ProcessSpecialInput);
	glutMouseFunc(ProcessMouseInput);

	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(DisplayFunc);

	Scene scene;

	Shader defaultShader("shaders/vertex.glsl", "shaders/fragment.glsl");

	const Player player(&defaultShader);

	scene.addChild(player);

	MainScene = &scene;
	glutMainLoop();

	return 0;
}