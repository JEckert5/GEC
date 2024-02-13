#define STB_IMAGE_IMPLEMENTATION
#include <GL/glut.h>
#include <IrrKlang/irrKlang.h>
#include <iostream>
#include <vector>
using namespace irrklang;

#define WIN_X 100
#define WIN_Y 100
#define WIN_H 600 // in pixels
#define WIN_W 600

bool ShowScreen = true;
bool ShowAxes = true;

static GLuint Texture;

float TexCoords[] = {
	0.0f, 0.0f, // Lower Left
	1.0f, 0.0f, // Lower Right
	1.0f, 1.0f, // Upper Right
	0.0f, 1.0f, // Upper Left
};

struct Audio {
	std::string fileName;
	ISoundEngine* engine;

	Audio(std::string fn) {
		fileName = fn;
		engine = createIrrKlangDevice();
	}

	void play() const {
		engine->play2D(fileName.c_str(), false);
	}

	void stop() const {
		engine->stopAllSounds();
	}
};

std::vector<Audio> SoundEngines;

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

void DrawShapes() {
	// Square
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 0);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.5, 0.25, 0);
	glVertex3f(0.75, 0.25, 0);
	glVertex3f(0.75, 0, 0);
	glVertex3f(0.5, 0, 0);
	glEnd();

	// Triangle
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 1, 0);
	glVertex3f(-0.25, 0, 0);
	glVertex3f(-0.375, 0.25, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-0.25, 0, 0);
	glEnd();
}

void DrawSprite() {
	glBegin(GL_LINE_LOOP);

	glEnd();
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (!ShowScreen)
		return;

	DrawAxes();

	DrawShapes();

	glFlush();

	Sleep(1); // My whole pc freezes if this isn't here lol.
}

void processInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		std::cout << "C pressed\n";
		ShowScreen = false;
		break;
	case 'm':
		std::cout << "M pressed\n";
		ShowScreen = true;
		break;
	case 'a':
		ShowAxes = !ShowAxes;
		break;
	case '1':
		SoundEngines[0].play();
		break;
	case '2':
		SoundEngines[1].play();
		break;
	case '3':
		SoundEngines[2].play();
		break;
	case 'p':
		for (auto& a: SoundEngines) 
			a.stop();
		
		break;
	case 27:
		exit(69);
	default:
		break;
	}
}

void processSpecialInput(int key, int x, int y) {
	switch (key) {
	default:
		break;
	}
}

void init(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); // RGBA mode

	glutInitWindowSize(WIN_W, WIN_H); // window size
	glutInitWindowPosition(WIN_X, WIN_Y);

	auto window = glutCreateWindow("Jens Eckert jeckert5");

	glClearColor(0.0, 0.0, 0.0, 1.0); // clear the window screen

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Sound Engine

	const std::string sounds[] = {
		"sounds/gas.mp3",
		"sounds/shmoney.mp3",
		"sounds/stinky.mp3"
	};

	for (const auto& sound : sounds) {
		SoundEngines.emplace_back(sound);
	}

	// Texture

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main(int argc, char** argv) {
	init(argc, argv);

	glutDisplayFunc(MyDisplay); // call the drawing function
	glutIdleFunc(MyDisplay);

	glutKeyboardFunc(processInput);
	glutSpecialFunc(processSpecialInput);

	glutMainLoop();

	return 0;
}