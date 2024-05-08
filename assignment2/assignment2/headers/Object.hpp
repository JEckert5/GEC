#pragma once

#ifndef RENDERED_OBJECT_HPP
#define RENDERED_OBJECT_HPP

#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include <vector>
#include <iostream>

class Object {
public:
	Object(Mesh mesh, Shader* shader, const std::vector<std::string>& textures);
	Object(const Mesh& mesh, Shader* shader, const std::string& texture);
	Object(const Mesh& mesh, Shader* shader, const Texture& texture);
	Object(Mesh mesh, Shader* shader, const std::vector<Texture>& textures);

	Object() = default;

	// Object(Object&& other);

	~Object();

	virtual void draw(const glm::mat4& view, const glm::mat4& proj);
	virtual void update(const double deltaTime, const double elapsedTime);
	Shader* getShader() const;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 collider;
	bool active = true;

protected:
	GLuint mVAO{}, mVBO{}, mEBO{};
	Mesh mMesh;
	glm::mat4 mTransMat;
	Shader* mShader;
	std::vector<Texture> mTextures;
	Texture mActiveTexture;
	unsigned mFrame = 0;
	float mFrameTimer = 0.f;
	void updateMatrix();
	virtual void setupBuffers();
	void setDefaults();
};

// 2D collision
inline bool CheckCollision(const Object& a, const Object& b) {
	const bool x = a.position.x + a.collider.x >= b.position.x && b.position.x + b.collider.x >= a.position.x;

	const bool y = a.position.y + a.collider.y >= b.position.y && b.position.y + b.collider.y >= a.position.y;

	return x && y;
}

inline bool CheckCollisionX(const Object& a, const Object& b) {
	return a.position.x + a.collider.x >= b.position.x && b.position.x + b.collider.x >= a.position.x;
}

inline bool CheckCollisionY(const Object& a, const Object& b) {
	return a.position.y + a.collider.y >= b.position.y && b.position.y + b.collider.y >= a.position.y;
}

inline float DistanceX(const Object& a, const Object& b) {
	return sqrtf(powf(b.position.x - a.position.x, 2));
}

inline float DistanceY(const Object& a, const Object& b) {
	return sqrtf(powf(b.position.y - a.position.y, 2));
}

inline float Distance(const Object& a, const Object& b) {
	return sqrtf(powf(b.position.x - a.position.x, 2) + powf(b.position.y - a.position.y, 2));
}

#endif
