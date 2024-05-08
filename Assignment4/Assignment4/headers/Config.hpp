#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Config {
	static constexpr double WindowW = 800;
	static constexpr double WindowH = 600;
	static constexpr double WindowX = 500;
	static constexpr double WindowY = 500;
	static constexpr double AspectRatio = WindowW / WindowH;
	inline static int window = 0;
	inline static bool PlayMode = false;
};

#endif