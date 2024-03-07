#include "Object.hpp"

Object::Object(const Mesh& mesh, Shader* shader, const std::string& texPath) {
	mMesh = mesh;

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(float), mMesh.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(float), mMesh.indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	mShader = shader;

	mTexture = texPath;

	mTransMat = glm::mat4(1.0f);
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	scale = glm::vec3(1);
}

Object::~Object() {
	mShader = nullptr;
}


void Object::update(const int deltaTime, const int elapsedTime) {
	mTransMat = glm::identity<glm::mat4>();
	mTransMat = glm::scale(mTransMat, scale);
	mTransMat = rotate(mTransMat, rotation.x, glm::vec3(1, 0, 0));
	mTransMat = rotate(mTransMat, rotation.y, glm::vec3(0, 1, 0));
	mTransMat = rotate(mTransMat, rotation.z, glm::vec3(0, 0, 1));
	mTransMat = translate(mTransMat, position);
}


void Object::draw() {
	mShader->use();

	mShader->setMat4("transform", mTransMat);
	mShader->setInt("uTexture", 0);

	glBindTexture(GL_TEXTURE_2D, mTexture.id);
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mMesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}

Shader* Object::getShader() const {
	return mShader;
}
