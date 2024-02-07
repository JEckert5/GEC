#include <GL/glut.h>

void MyDisplay()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 0);
	glVertex3f(0.5, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0.5, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0.5);
	glEnd();
	glFlush();
	glutSwapBuffers();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	int width = 1920;
	int height = 1080;
	glutInitWindowSize(width, height);

	int x = 200;
	int y = 100;
	glutInitWindowPosition(x, y);

	int win = glutCreateWindow("Draw Triangle");

	glutDisplayFunc(MyDisplay);

	glutMainLoop();

	return 0;
}