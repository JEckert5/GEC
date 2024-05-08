#include "Object.hpp"

Object::Object(Mesh mesh, Shader* shader, const std::vector<std::string>& textures): mMesh(std::move(mesh)), mShader(shader) {
	for (auto& p : textures)
		mTextures.emplace_back(p);

	setDefaults();
}

Object::Object(Mesh mesh, Shader* shader, const std::vector<Texture>& textures): mMesh(std::move(mesh)), mShader(shader), mTextures(textures) {
	setDefaults();
}

Object::Object(const Mesh& mesh, Shader* shader, const std::string& texture) : Object(mesh, shader, std::vector({texture})) {}

Object::Object(const Mesh& mesh, Shader* shader, const Texture& texture) : Object(mesh, shader, std::vector({ texture })) {}

Object::~Object() {
	mShader = nullptr;
}

void Object::update(const double deltaTime, const double elapsedTime) {

}


void Object::draw(const glm::mat4& view, const glm::mat4& proj) {
	if (!active) return;

	updateMatrix();

	mShader->use();
	mShader->setMat4("transform", mTransMat);
	mShader->setMat4("view", view);
	mShader->setMat4("projection", proj);

	glBindTexture(GL_TEXTURE_2D, mActiveTexture.id);
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mMesh.indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mShader->disable();
}

Shader* Object::getShader() const {
	return mShader;
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

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(GLuint), mMesh.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(1);
}

void Object::setDefaults() {
	setupBuffers();

	mActiveTexture = mTextures[0];

	mTransMat = glm::identity<glm::mat4>();
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	scale = glm::vec3(1);
	collider = glm::vec3(1);
}
