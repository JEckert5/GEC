#include "Object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <sstream>
#include "Config.hpp"

Object::Object(std::string name, Mesh mesh, const Texture& texture, const bool isSprite) :
	position(0),
	scale(1),
	rotation(0),
	collider(1),
	name(std::move(name)),
	mesh(std::move(mesh)),
	texture(texture),
	m_transMat(glm::identity<glm::mat4>()) {

}

Object Object::create(const std::string& name, const Texture& texture, const Mesh& mesh) {
	Object o(name, mesh, texture);
	o.setDefaults();

	o.boxCollider.SetAsBox(0.5f, 0.5f, { 0, 0 }, 0);

	return o;
}

void Object::update() {
	
}

void Object::onCollision(const Object& other) {
	
}

void Object::onCollision(const b2EdgeShape* edge) {
	
}

std::string Object::saveString() {
	std::stringstream stream;

	stream << type << " " << name << " " <<
		position.x << " " << position.y << " " << position.z << " " <<
		scale.x << " " << scale.y << " " << scale.z << " " <<
		rotation.x << " " << rotation.y << " " << rotation.z << " " <<
		texture.fileName << " " <<
		collision << " " << hasGravity << " " << gravity;

	return stream.str();
}

void Object::draw(const glm::mat4& view, const glm::mat4& proj) {
	if (!active)
		return;

	updateMatrix();

	ShaderLib::Default.use();
	ShaderLib::Default.setMat4("transform", m_transMat);
	ShaderLib::Default.setMat4("view", view);
	ShaderLib::Default.setMat4("projection", proj);

	glBindTexture(GL_TEXTURE_2D, texture.id);

	mesh.draw();

	// glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Shader::unbind();
}

void Object::setDefaults() {
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	scale = glm::vec3(1);
	collider = glm::vec3(0.5f);
	colliderOffset = glm::vec3(0);
}

void Object::updateMatrix() {
	m_transMat = glm::identity<glm::mat4>();

	m_transMat = translate(m_transMat, position);
	m_transMat = rotate(m_transMat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	m_transMat = rotate(m_transMat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	m_transMat = rotate(m_transMat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	m_transMat = glm::scale(m_transMat, scale);
}

void Object::setTexture(const std::string& path) {
	auto t = Texture::create(path);

	texture = t;
}

void Object::setTexture(const Texture& t) {
	texture = t;
}
