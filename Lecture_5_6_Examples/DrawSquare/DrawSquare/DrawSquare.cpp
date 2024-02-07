#include <GL/glut.h>

void MySquareDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex3f(0.25, 0.25, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0.75, 0.25, 0);
	glColor3f(0, 0, 1);
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

	int win = glutCreateWindow("Draw Square");

	init();

	glutDisplayFunc(MySquareDisplay);

	glutMainLoop();

	return 0;
}