#pragma once
#include "GL/glew.h"
#include "glm/vec2.hpp"

#include <memory>

class Enemy;

struct HealthBar {

	HealthBar(float x, float y, float w, float h, const std::shared_ptr<Enemy>& target): tracked(target), x(x), y(y), width(w), height(h), initial(target->health) { }

	void draw() {
		const float top = height / 2;
		const float bottom = -height / 2;

		glPushMatrix();

		glColor3f(1, 1, 1);

		glTranslatef(x, y, 0);
		glScalef(1.15f, 1.15f, 1);

		glBegin(GL_POLYGON);
		glVertex2f(-width / 2, bottom);
		glVertex2f(width / 2, bottom);
		glVertex2f(width / 2, top);
		glVertex2f(-width / 2 , top);
		glEnd();


		glPopMatrix();

		glPushMatrix();

		glColor3f(1, 0.2f, 0.2f);
		glTranslatef(x, y, 0);

		const float right = -width / 2 + width * (tracked->health / initial);

		glBegin(GL_POLYGON);
		glVertex2f(-width / 2, bottom);
		glVertex2f(right, bottom);
		glVertex2f(right, top);
		glVertex2f(-width / 2, top);
		glEnd();

		glPopMatrix();
	}

	std::shared_ptr<Enemy> tracked;
	float x, y;
	float width, height;
	float initial;
};

