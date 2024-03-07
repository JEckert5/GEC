#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Object.hpp"
#include "Audio.hpp"

class Scene {
public:
	Scene() = default;

	void addChild(const Object& child);
	void removeChild(const std::vector<Object>::iterator& titerator);
	void removeChild(const int i);
	Object& popChild();

	void addAudio(const Audio& audio);
	void playAudio(const int i);
	void stopAudio(const int i);
	void resumeAudio(const int i);
	bool isAudioPlaying(const int i);

	void update(const int deltaTime, const int elapsedTime);
	void draw();
private:
	std::vector<Object> mChildren;
	std::vector<Audio> mAudios;
};

#endif