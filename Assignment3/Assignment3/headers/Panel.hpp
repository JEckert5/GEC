#pragma once
#ifndef PANEL_HPP
#define PANEL_HPP
#include <vector>
#include <glm/glm.hpp>

#include "Button.hpp"

class Panel {
public:
	
	Panel(glm::ivec2 pos, glm::ivec2 wh, glm::vec3 color);
	Panel() = default;

	void addButton(Button&& button);
	void addButton(Button& button);

	void addInput(TextInput&& input);
	void addInput(TextInput& input);

	bool inside(int x, int y);

	void draw();

	bool active = true;

	std::vector<Button> buttons;
	std::vector<Button> sections;
	std::vector<TextInput> inputs;
	std::vector<Object> objects;

	float scroll;
private:
	glm::ivec2 mPos;
	glm::ivec2 mDimensions;
	glm::vec3 mColor;
};

#endif