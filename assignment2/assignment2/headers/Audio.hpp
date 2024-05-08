#pragma once

#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string>

#include "IrrKlang/irrKlang.h"

struct Audio {
	Audio() = delete;

	Audio(std::string path, const float vol = 1, const bool loop = false): file(std::move(path)), volume(vol), looped(loop) {
		engine = irrklang::createIrrKlangDevice();
		engine->setSoundVolume(volume);
	}

	~Audio() {
		delete engine;
	}

	void start() const {
		engine->play2D(file.c_str(), looped);
	}

	void stop() const {
		engine->stopAllSounds();
	}

	void pause() {
		engine->setAllSoundsPaused();
		paused = true;
	}

	void resume() {
		engine->setAllSoundsPaused(false);
		paused = false;
	}

	std::string file;
	irrklang::ISoundEngine* engine;
	float volume;
	bool looped;
	bool paused = false;
};

#endif