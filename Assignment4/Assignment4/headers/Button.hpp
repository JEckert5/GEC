#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Text.hpp"
#include <string>
#include <sstream>
#include <functional>

struct Button {
	float x, y; // Center
	float width, height;
	float winX, winY;
	float winW, winH;
	bool active = true;
	glm::vec3 color;
	Text text;
	std::function<void()> action;

	Button(
		float x, float y, 
		float w, float h, 
		float winX, float winY,
		std::string str, 
		std::function<void()> onClick
	): 
		x(x), y(y), 
		width(w), height(h), 
		winX(winX), winY(winY), 
		winW(w), winH(h),
		color(0.9f, 0.9f, 0.9f), text(std::move(str), "JetBrainsMono", {0, 0}, {0, 0, 0}),
		action(std::move(onClick)),
		m_ogTextY(0),
		m_ogWinY(winY) {
		float cx = x - width / 2;
		float cy = y - height / 2;

		text.position.x = cx + w / 16;
		text.position.y = cy + height / 4;

		m_ogTextY = text.position.y;
	}

	Button() = default;

	bool inside(int mouseX, int mouseY) const {
		float lb = winX - winW / 2;
		float rb = winX + winW / 2;
		float bb = winY - winH / 2;
		float tb = winY + winH / 2;

		bool xin = mouseX >= lb && mouseX <= rb;
		bool yin = mouseY >= bb && mouseY <= tb;

		return xin && yin;
	}

	void click() const {
		if (action && active)
			action();
	}

	void draw() const {
		if (!active)
			return;

		// Bottom left
		float cx = x - width / 2;
		float cy = y - height / 2;

		glColor3f(color.r, color.g, color.b);
		glBegin(GL_QUADS);
		glVertex3f(cx, cy, 0);
		glVertex3f(cx + width, cy, 0);
		glVertex3f(cx + width, cy + height, 0);
		glVertex3f(cx, cy + height, 0);
		glEnd();

		text.draw();
	}

	void scroll(const float s) {
		winY = m_ogWinY - s;
		text.position.y = m_ogTextY + s;
	}

private:
	float m_ogTextY, m_ogWinY;
};

struct TextInput {
	enum class TrackedVal {
		POS,
		SCALE
	};

	float x, y; // Center
	float width, height;
	float winX, winY;
	float winW, winH;
	glm::vec3 color;
	glm::vec3 topColor;
	Text title, value;
	TrackedVal trackedVal;
	std::function<void(TextInput*)> action;
	Object* object;
	bool active = false;

	TextInput(float x, float y,
			  float w, float h,
			  float winX, float winY,
			  std::string titl,
			  TrackedVal t,
			  Object* obj,
			  std::function<void(TextInput*)> onUpdate)
	    :x(x), y(y),
		width(w), height(h),
		winX(winX), winY(winY),
		winW(w), winH(h),
		color(0.9f, 0.9f, 0.9f),
		topColor(0.85f, 0.85f, 0.85f),
		title(std::move(titl), "JetBrainsMono", { 0, 0 }, { 0, 0, 0 }),
		value("", "JetBrainsMono", {0, 0}, {0, 0, 0}),
		trackedVal(t),
		action(std::move(onUpdate)),
		object(obj)
	{
		float cx = x - width / 2;
		float cy = y - height / 2;

		title.position.x = cx + width / 16;
		title.position.y = cy + height * 1.05f;

		value.position.x = cx + width / 16;
		value.position.y = cy + height / 4;
	}

	TextInput() = default;

	virtual ~TextInput() {
		object = nullptr;
	}

	// virtual void action() = 0;

	bool inside(int mouseX, int mouseY) const {
		float lb = winX - winW / 2;
		float rb = winX + winW / 2;
		float bb = winY - winH / 2;
		float tb = winY + winH / 2;

		bool xin = mouseX >= lb && mouseX <= rb;
		bool yin = mouseY >= bb && mouseY <= tb;

		return xin && yin;
	}

	void draw() const {
		// TODO: Make it use shaders/mesh

		// Bottom left
		float cx = x - width / 2;
		float cy = y - height / 2;

		glColor3f(topColor.r, topColor.g, topColor.b);
		glBegin(GL_QUADS);
		glVertex3f(cx, cy + height, 0);
		glVertex3f(cx + width, cy + height, 0);
		glVertex3f(cx + width, cy + height + height / 2, 0);
		glVertex3f(cx, cy + height + height / 2, 0);
		glEnd();

		// Input field
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_QUADS);
		glVertex3f(cx, cy, 0);
		glVertex3f(cx + width, cy, 0);
		glVertex3f(cx + width, cy + height, 0);
		glVertex3f(cx, cy + height, 0);
		glEnd();

		title.draw();
		value.draw(); 
	}

	void keyPress(unsigned char key) {
		if (!active || object == nullptr)
			return;

		if (key == '\b' && !value.str.empty()) {
			value.str.pop_back();

			if (value.str.empty()) {
				// value.str += "0";
			}
		} else if ((key >= '0' && key <= '9') || key == '.' || key == '-') {
			value.str += key;
		}

		if (object != nullptr) {
			action(this);
		}
	}

	[[nodiscard]] float valToFloat() const {
		float r;
		std::stringstream ss(value.str);
		ss >> r;
		return r;
	}
};


#endif