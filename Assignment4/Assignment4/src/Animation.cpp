#include "Animation.hpp"

#include <sstream>

#include "Config.hpp"
#include "Time.hpp"

Animation::Animation(const Texture& texture, int frames, double frameRate, double xOffset, double yOffset, double width, double height):
	m_nFrames(frames),
	m_frameRate(frameRate),
	m_xOffset(xOffset),
	m_yOffset(yOffset),
	m_width(width),
	m_height(height),
	m_texture(texture) {
	genFrames();
}

Animation::Animation(const AnimData& data):
	m_frames({}),
	m_nFrames(data.nFrames),
	m_frameRate(data.frameRate),
	m_xOffset(data.xOffset),
	m_yOffset(data.yOffset),
	m_width(data.width),
	m_height(data.height),
	m_texture(data.t) {
	genFrames();
}

Animation::Animation(const std::string& texture, int frames, double frameRate, double xOffset, double yOffset, double width, double height):
	m_nFrames(frames),
	m_frameRate(frameRate),
	m_xOffset(xOffset),
	m_yOffset(yOffset),
	m_width(width),
	m_height(height),
	m_texture(Texture::create(texture)) {
	genFrames();
}

void Animation::genFrames() {
	const double top = 1.0 - m_yOffset;
	const double bottom = 1.0 - (m_yOffset + m_height);

	for (int i = 0; i < m_nFrames; i++) {
		const double right = m_xOffset + (m_width + m_width * i);
		const double left = m_xOffset + m_width * i;

		glm::vec2 bl = { left, bottom };
		glm::vec2 br = { right, bottom };
		glm::vec2 tr = { right, top };
		glm::vec2 tl = { left, top };

		// Clockwise
		m_frames.emplace_back(std::vector{ tr, br, bl, tl });
	}
}

void Animation::update() {
	m_frameTimer += Time::deltaTime;

	if (m_frameTimer < (1.0f / m_frameRate))
		return;

	m_frame += 1;
	m_frame %= m_nFrames;

	m_frameTimer = 0;
}

void Animation::bindTexture() const {
	glBindTexture(GL_TEXTURE_2D, m_texture.id);
}

std::string Animation::saveString() {
	std::stringstream stream;

	stream << m_texture.fileName << " " << m_nFrames << " " << m_frameRate << " " << m_xOffset << " " << m_yOffset << " " << m_width << " " << m_height;

	return stream.str();
}

std::vector<glm::vec2> Animation::getCurrentFrame() {
	return m_frames[m_frame];
}
