#include "AnimationManager.hpp"

#include <sstream>

void AnimationManager::setState(const std::string& state) {
	if (!m_anims.contains(state))
		throw std::runtime_error("No animations with state " + state + " exists!\n");

	m_state = state;
}

std::string AnimationManager::saveString() {
	std::stringstream stream;

	for (auto i = m_anims.begin(); i != m_anims.end(); ++i) {
		stream << i->first << " " << i->second.saveString() << " ";
	}

	return stream.str();
}

Animation* AnimationManager::getCurrentAnimation() {
	return &m_anims[m_state];
}

std::string AnimationManager::getCurrentAnimationState() {
	return m_state;
}

// Returns nullptr if no animation with that state exists.
Animation* AnimationManager::getAnimationFromState(const std::string& state) {
	if (m_anims.contains(state))
		return &m_anims[state];

	return nullptr;
}

void AnimationManager::addAnimation(const std::string& state, const Animation& anim) {
	m_anims.insert(std::make_pair(state, anim));
}


