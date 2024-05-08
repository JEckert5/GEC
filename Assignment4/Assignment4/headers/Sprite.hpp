#pragma once

#include "AnimationManager.hpp"
#include "Object.hpp"

class Sprite : public Object {
public:
	Sprite(const std::string& name, int columns, double xOffset, double yOffset, double width, double height, double frameTime, const Texture& texture);
	Sprite(const std::string& name, const std::vector<AnimData>& anims);

	Sprite(const std::string& name);

	void update() override;
	void draw(const glm::mat4& view, const glm::mat4& proj) override;

protected:
	AnimationManager m_animManager;
};