#pragma once
#include "Texture.hpp"
#include "glm/vec2.hpp"

struct AnimData {
	Texture t;
	int nFrames;
	double frameRate, xOffset, yOffset, width, height;
};

// A class for 2D animation
class Animation {
public:
	Animation() = default;
	explicit Animation(const std::string& texture, int frames, double frameRate, double xOffset, double yOffset, double width, double height);
	explicit Animation(const Texture& texture, int frames, double frameRate, double xOffset, double yOffset, double width, double height);
	explicit Animation(const AnimData& data);

	void update();
	void bindTexture() const;
	std::string saveString();

	std::vector<glm::vec2> getCurrentFrame();
private:
	void genFrames();

	// Precalculated texture coordinates for each frame.
	std::vector<std::vector<glm::vec2>> m_frames;
	int m_nFrames, m_frame = 0;
	double m_frameRate, m_frameTimer = 0.f;
	double m_xOffset, m_yOffset;
	double m_width, m_height;
	Texture m_texture;
};
