#pragma once

#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string>

#include "IrrKlang/irrKlang.h"

struct Audio {
	Audio() = default;

	Audio(std::string path, const float vol = 1, const bool loop = false): file(std::move(path)), volume(vol), looped(loop) {
		engine = irrklang::createIrrKlangDevice();
		engine->setSoundVolume(volume);
	}

	~Audio() {
		delete engine;
	}

	void start() {
		engine->play2D(file.c_str(), looped);
		playing = true;
	}

	void stop() const {
		engine->stopAllSounds();
	}

	void pause() {
		engine->setAllSoundsPaused();
		playing = false;
	}

	void resume() {
		engine->setAllSoundsPaused(false);
		playing = true;
	}

	[[nodiscard]] bool isPlaying() const {
		return playing;
	}

	std::string file;
	irrklang::ISoundEngine* engine;
	float volume;
	bool looped;
	bool playing = false;
};

#endif