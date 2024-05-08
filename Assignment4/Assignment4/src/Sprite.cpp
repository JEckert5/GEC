#include "Sprite.hpp"
#include "Time.hpp"

Sprite::Sprite(const std::string& name, int columns, double xOffset, double yOffset, double width, double height, double frameTime, const Texture& texture)
	: Object(name, DefaultMesh::SpriteSquare, texture, true) {

	type = SPRITE;
}

Sprite::Sprite(const std::string& name): Object(name, DefaultMesh::SpriteSquare, Texture::create("")) {
	type = SPRITE;

	Texture::destroy(texture);
}

void Sprite::update() {
	auto anim = m_animManager.getCurrentAnimation();

	anim->update();

	auto frame = anim->getCurrentFrame();

	mesh.vertices[0].texCoords = frame[0];
	mesh.vertices[1].texCoords = frame[1];
	mesh.vertices[2].texCoords = frame[2];
	mesh.vertices[3].texCoords = frame[3];

	anim = nullptr;
}

void Sprite::draw(const glm::mat4& view, const glm::mat4& proj) {
	if (!active)
		return;

	updateMatrix();

	ShaderLib::Default.use();
	ShaderLib::Default.setMat4("transform", m_transMat);
	ShaderLib::Default.setMat4("view", view);
	ShaderLib::Default.setMat4("projection", proj);

	m_animManager.getCurrentAnimation()->bindTexture();

	mesh.draw();

	Shader::unbind();
}

