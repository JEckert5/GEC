#pragma once

#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string>

#include "IrrKlang/irrKlang.h"

struct Audio {
	Audio(): volume(1), looped(false) {
		engine = irrklang::createIrrKlangDevice();
		engine->setSoundVolume(volume);
	}

	Audio(const Audio& other): file(other.file), volume(other.volume), looped(other.looped) {
		engine = irrklang::createIrrKlangDevice();
		engine->setSoundVolume(volume);
	}

	Audio(std::string path, const float vol = 1, const bool loop = false): file(std::move(path)), volume(vol), looped(loop) {
		engine = irrklang::createIrrKlangDevice();
		engine->setSoundVolume(volume);
	}

	~Audio() {
		delete engine;
	}

	Audio& operator=(const Audio& rhs) {
		if (this == &rhs)
			return *this;

		if (engine == nullptr)
			engine = irrklang::createIrrKlangDevice();

		volume = rhs.volume;
		file = rhs.file;
		looped = rhs.looped;

		engine->setSoundVolume(volume);

		return *this;
	}

	Audio& operator=(const std::string& rhs) {
		if (engine == nullptr)
			engine = irrklang::createIrrKlangDevice();

		file = rhs;

		return *this;
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
		return engine->isCurrentlyPlaying(file.c_str());
	}

	std::string file;
	irrklang::ISoundEngine* engine;
	float volume;
	bool looped;
	bool playing = false;
};

#endif