#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "Animation.hpp"


class AnimationManager {
public:
	AnimationManager() = default;

	void setState(const std::string& state);

	std::string saveString();

	Animation* getCurrentAnimation();
	std::string getCurrentAnimationState();
	Animation* getAnimationFromState(const std::string& state);
	void addAnimation(const std::string& state, const Animation& anim);
private:
	std::string m_state;
	std::unordered_map<std::string, Animation> m_anims;
};
