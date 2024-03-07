#include <GL/glut.h>
#include <IrrKlang/irrKlang.h>
#include <FreeImage/FreeImage.h>
#include <iostream>
#include <vector>
using namespace irrklang;

#define WIN_X 100
#define WIN_Y 100
#define WIN_H 600 // in pixels
#define WIN_W 600

bool ShowScreen = true;
bool ShowAxes = true;

GLuint Textures[2];

std::string TextureFiles[2] = {
	"img/saber_1.png",
	"img/saber_2.png"
};

int Frame = 0;

float CamZoom = 1.0f;
float MoveX = 0, MoveY = 0, Speed = 1;

bool Moved = false;
bool ColorToggle = false;
bool Pause = false;

struct Audio {
	std::string fileName;
	ISoundEngine* engine;

	Audio(std::string fn) {
		fileName = fn;
		engine = createIrrKlangDevice();
		engine->setSoundVolume(0.3f);
	}

	void play() const {
		engine->play2D(fileName.c_str(), true);
	}

	void stop() const {
		engine->stopAllSounds();
	}
};

int SoundIndex = 0;
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
	if (!ColorToggle)
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 1, 0);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.5, 0.25, 0);
	glVertex3f(0.75, 0.25, 0);
	glVertex3f(0.75, 0, 0);
	glVertex3f(0.5, 0, 0);
	glEnd();

	// Triangle
	glBegin(GL_LINE_LOOP);
	if (!ColorToggle)
		glColor3f(0, 1, 0);
	else
		glColor3f(1, 0, 0);
	glVertex3f(-0.25, 0, 0);
	glVertex3f(-0.375, 0.25, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-0.25, 0, 0);
	glEnd();
}

void DrawSprite() {
	// Translations
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(MoveX, MoveY, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Textures[Frame]);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(-0.35f, -0.35f, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.35f, -0.35f, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.35f, 0.35f, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.35f, 0.35f, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	
}

void MyDisplay() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	if (!ShowScreen)
		goto flush;

	DrawAxes();

	DrawShapes();

	DrawSprite();

	flush:
	glFlush();
	glutSwapBuffers();

	Sleep(1); // My whole pc freezes if this isn't here lol.
}

void ProcessInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		ShowScreen = false;
		break;
	case 'm':
		ShowScreen = true;
		break;
	case 'a':
		ShowAxes = !ShowAxes;
		break;
	case 'p':
		if (!Pause) {
			Pause = true;

			for (auto& a : SoundEngines)
				a.stop();
		} else {
			Pause = false;

			SoundEngines[SoundIndex].play();
		}
		
		break;
	case 27:
		exit(69);
	default:
		break;
	}
}

void ProcessSpecialInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		MoveX += -0.1f;
		Moved = true;
		break;
	case GLUT_KEY_RIGHT:
		MoveX += 0.1f;
		Moved = true;
		break;
	case GLUT_KEY_UP:
		MoveY += 0.1f;
		Moved = true;
		break;
	case GLUT_KEY_DOWN:
		MoveY += -0.1f;
		Moved = true;
		break;
	default:
		break;
	}
}

void ProcessMouseInput(int button, int state, int x, int y) {
	if (button == 0) { // Left mouse
		if (state == GLUT_UP) {
			ColorToggle = !ColorToggle;
		}
		return;
	}

	if (button == 2) { // Right mouse
		if (state == GLUT_UP) {
			SoundEngines[SoundIndex].stop();

			SoundIndex++;

			if (SoundIndex >= 3)
				SoundIndex = 0;

			SoundEngines[SoundIndex].play();
		}

		return;
	}

	if (button == 3) { // Scroll up

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (state == GLUT_UP) {
			CamZoom += 0.1f;
			glOrtho(-1.f / CamZoom, 1.f / CamZoom, -1.f / CamZoom, 1.f / CamZoom, -1, 1);
		}
	}

	if (button == 4) { // Scroll down

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (state == GLUT_UP) {
			CamZoom -= 0.1f;
			glOrtho(-1.f / CamZoom, 1.f / CamZoom, -1.f / CamZoom, 1.f / CamZoom, -1, 1);
		}
	}

	// glTranslatef(0, 0, 0);
}

void Init(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA); // RGBA mode

	glutInitWindowSize(WIN_W, WIN_H); // window size
	glutInitWindowPosition(WIN_X, WIN_Y);

	glutCreateWindow("Jens Eckert jeckert5");

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
	glGenTextures(2, Textures);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable transparent textures
	glEnable(GL_BLEND);

	for (int i = 0; i < 2; i++) {
		const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(TextureFiles[i].c_str());

		if (format == FIF_UNKNOWN) {
			printf("Unknown file type for texture image file %s\n", TextureFiles[i].c_str());
			continue;
		}

		FIBITMAP* bitmap = FreeImage_Load(format, TextureFiles[i].c_str());

		if (!bitmap) {
			printf("Failed to load image %s\n", TextureFiles[i].c_str());
			continue;
		}

		FIBITMAP* bm2 = FreeImage_ConvertTo32Bits(bitmap); // BGRA == 32 bits
		FreeImage_Unload(bitmap);
		void* data = FreeImage_GetBits(bm2);
		const int width = FreeImage_GetWidth(bm2);
		const int height = FreeImage_GetHeight(bm2);

		if (!data) {
			printf("Failed to get data from bitmap %s\n", TextureFiles[i].c_str());
			continue;
		}

		printf("Texture loaded, %s, %i, %i", TextureFiles[i].c_str(), width, height);

		glBindTexture(GL_TEXTURE_2D, Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	SoundEngines[SoundIndex].play();
}

void FrameTimer(int v) {
	Frame++;

	if (Frame >= 2) {
		Frame = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(100, FrameTimer, v);
}

int main(int argc, char** argv) {
	Init(argc, argv);

	glutTimerFunc(0, FrameTimer, 0);

	glutDisplayFunc(MyDisplay);
	glutIdleFunc(MyDisplay);

	glutKeyboardFunc(ProcessInput);
	glutSpecialFunc(ProcessSpecialInput);
	glutMouseFunc(ProcessMouseInput);

	glutMainLoop();

	return 0;
}