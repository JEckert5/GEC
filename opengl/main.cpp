#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>

#define WIN_X 100
#define WIN_Y 100
#define WIN_H 600
#define WIN_W 600

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINE_LOOP);
    glColor3f(1, 0, 0);
    glVertex3f(0.5, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0.5, 0);
    glColor3f(0, 0, 1);
    glVertex3f(-0.5, 0, 0);
    glEnd();
    glFlush();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(WIN_X, WIN_Y);
    glutCreateWindow("Jens Eckert jeckert5");

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
    glutDisplayFunc(displayFunc);
    glutMainLoop();

    return 0;
}