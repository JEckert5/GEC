#include <GL/glut.h>

float gRed = 1.0;
float gGreen = 1.0; 
float gBlue = 1.0;

void mySquareDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glColor3f(gRed, 0, 0);
	glVertex3f(0.25, 0.25, 0);
	glColor3f(0, gGreen, 0);
	glVertex3f(0.75, 0.25, 0);
	glColor3f(0, 0, gBlue);
	glVertex3f(0.75, 0.75, 0);
	glColor3f(1, 1, 1);
	glVertex3f(0.25, 0.75, 0);
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void init(void) 
{
	GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);

	// Applies subsequent matrix operations to the projection matrix stack.
	glMatrixMode(GL_PROJECTION);
	// Replace current matrix with identity matrix
	glLoadIdentity();
	// Orthographic projection matrix
	glOrtho(0, 1, 0, 1, -1, 1);
}

void procSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
			gRed += .1;
			break;
		case GLUT_KEY_F2:
			gGreen += .1;
			break;
		case GLUT_KEY_F3:
			gBlue += .1;
			break;
	}
}

void procKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'r':
			gRed -= .1;
			break;
		case 'g':
			gGreen -= .1;
			break;
		case 'b':
			gBlue -= .1;
			break;
		case 27: // escape
			exit(0);
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	int width = 500;
	int height = 500;
	glutInitWindowSize(width, height);

	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);

	int win = glutCreateWindow("Key Input Example");

	// Use created render callback function
	glutDisplayFunc(mySquareDisplay);
	glutIdleFunc(mySquareDisplay);

	// Key input callback
	glutKeyboardFunc(procKeys);
	glutSpecialFunc(procSpecialKeys);

	glutMainLoop();

	return 0;
}

