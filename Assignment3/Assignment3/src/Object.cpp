#include "Object.hpp"

#include <glm/ext/matrix_transform.hpp>

Object::Object(std::string name, Mesh mesh, const std::vector<Texture>& textures, const bool isSprite):
	position(0),
	scale(1),
	rotation(0),
	collider(1),
	name(std::move(name)),
	mMesh(std::move(mesh)),
	mTextures(textures),
	mActiveTexture(mTextures[0]),
	mTransMat(0),
	mIsSprite(isSprite)
{
	setupBuffers();
	setDefaults();
}

Object Object::create(const std::string& name, const std::vector<Texture>& textures, const Mesh& mesh) {
	Object o(name, mesh, textures);

	return o;
}

void Object::update() {
	position += velocity;
}

void Object::draw(const glm::mat4& view, const glm::mat4& proj) {
	updateMatrix();

	ShaderLib::Default.use();
	ShaderLib::Default.setMat4("transform", mTransMat);
	ShaderLib::Default.setMat4("view", view);
	ShaderLib::Default.setMat4("projection", proj);

	glBindTexture(GL_TEXTURE_2D, mActiveTexture.id);
	glBindVertexArray(mVAO);

	if (mIsSprite) {
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mMesh.indices.size() * sizeof(GLuint), mMesh.indices.data());
	}

	glDrawElements(GL_TRIANGLES, mMesh.indices.size(), GL_UNSIGNED_INT, nullptr);

	// glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Shader::unbind();
}

void Object::setDefaults() {
	mActiveTexture = mTextures[0];
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	scale = glm::vec3(1);
	collider = glm::vec3(0.5f);
	colliderOffset = glm::vec3(0);
}

void Object::updateMatrix() {
	mTransMat = glm::identity<glm::mat4>();

	mTransMat = translate(mTransMat, position);
	mTransMat = rotate(mTransMat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	mTransMat = rotate(mTransMat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	mTransMat = rotate(mTransMat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	mTransMat = glm::scale(mTransMat, scale);
}

void Object::setupBuffers() {
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	if (mIsSprite) {
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(GLuint), mMesh.indices.data(), GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(1);
}

bool Object::isSprite() const {
	return mIsSprite;
}
