#include "Panel.hpp"

#include <glm/ext/matrix_transform.hpp>

Panel::Panel(const glm::ivec2 pos, const glm::ivec2 wh, const glm::vec3 color): m_pos(pos), m_dimensions(wh), m_color(color), scroll(0) {}

void Panel::draw() {
	if (!active)
		return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glViewport(m_pos.x, m_pos.y, m_dimensions.x, m_dimensions.y);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, m_dimensions.x, 0, m_dimensions.y, -1, 1);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glBegin(GL_POLYGON);
	glScalef(1,1, 1);
	glTranslatef(0, 0, 0);
	glColor3f(m_color.r, m_color.g, m_color.b);
	glVertex3f(0, 0, 0);
	glVertex3f(m_dimensions.x, 0, 0);
	glVertex3f(m_dimensions.x, m_dimensions.y, 0);
	glVertex3f(0, m_dimensions.y, 0);
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, scroll, 0);

	for (auto& button : buttons) {
		// keep original y position so that it doesn't run offscreen.
		button.scroll(scroll);

		if (!inside(m_pos.x + m_dimensions.x/2, button.winY)) {
			button.winY = -600;
		}

		button.draw();
	}

	for (auto& input: inputs) {
		// input.title.position.y += scroll;
		input.value.position.y += scroll;
		input.draw();
	}

	for (auto& o: objects) {
		// o.position.y += scroll;
		o.update();
		o.draw(glm::identity<glm::mat4>(), glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));
	}

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Panel::addButton(Button&& button) {
	button.text.setProjection(glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));

	buttons.emplace_back(std::move(button));
}

void Panel::addButton(Button& button) {
	button.text.setProjection(glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));

	buttons.emplace_back(button);
}

void Panel::addInput(TextInput&& input) {
	input.title.setProjection(glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));
	input.value.setProjection(glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));
	inputs.emplace_back(std::move(input));
}

void Panel::addInput(TextInput& input) {
	input.title.setProjection(glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));
	input.value.setProjection(glm::ortho(0.f, static_cast<float>(m_dimensions.x), 0.f, static_cast<float>(m_dimensions.y), -1.f, 1.f));
	inputs.emplace_back(input);
}

bool Panel::inside(int x, int y) {
	float topy = Config::WindowH - (m_pos.y + m_dimensions.y);

	bool xin = x >= m_pos.x && x <= m_dimensions.x + m_pos.x;
	bool yin = y >= topy && y <= m_dimensions.y + topy;

	return xin && yin;
}
