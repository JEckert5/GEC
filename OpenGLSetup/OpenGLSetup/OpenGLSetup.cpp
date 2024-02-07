#include <GL/glut.h>
#define WIN_X 100
#define WIN_Y 100
#define WIN_H 600 // in pixels
#define WIN_W 600

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Code here

	glFlush();
}
int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB); // RGB mode
	glutInitWindowSize(WIN_W, WIN_H); // window size
	glutInitWindowPosition(WIN_X, WIN_Y);
	glutCreateWindow("(Name here) (KSUID here)");

	glClearColor(0.0, 0.0, 0.0, 1.0); // clear the window screen
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay); // call the drawing function
	glutMainLoop();
	return 0;
}