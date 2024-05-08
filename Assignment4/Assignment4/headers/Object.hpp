#pragma once

#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <vector>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <box2d/b2_polygon_shape.h>

#include "Animation.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class Object {
public:
	enum ObjectType {
		OBJECT,
		SPRITE,
		PLAYER,
		ENEMY,
		COIN,
	};

	Object() = default;
	Object(std::string name, Mesh mesh, const Texture& texture, bool isSprite = false);
	static Object create(const std::string& name, const Texture& texture, const Mesh& mesh = DefaultMesh::Square);

	virtual void update();
	virtual void draw(const glm::mat4& view, const glm::mat4& proj);
	void setTexture(const std::string& path);
	void setTexture(const Texture& t);

	virtual void onCollision(const Object& other);
	virtual void onCollision(const b2EdgeShape* edge);

	virtual std::string saveString();

	b2PolygonShape boxCollider;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 collider;
	glm::vec3 colliderOffset;

	float gravity = 0;
	bool active = true;
	bool hasGravity = true;
	bool collision = true;
	std::string name;
	ObjectType type = ObjectType::OBJECT;
	Mesh mesh;
	Texture texture;

protected:
	glm::mat4 m_transMat;
	void updateMatrix();
	void setDefaults();

};


// 2D collision //////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool CheckCollision(const Object& a, const Object& b, const bool see = false) {
	if (!a.collision || !b.collision)
		return false;

	const float leftA = a.position.x + a.colliderOffset.x - a.collider.x / 2;
	const float bottomA = a.position.y + a.colliderOffset.y - a.collider.y / 2;

	const float leftB = b.position.x + b.colliderOffset.x - b.collider.x / 2;
	const float bottomB = b.position.y + b.colliderOffset.y - b.collider.y / 2;

	if (see) {
		glBegin(GL_LINES);
		glVertex3f(leftA, bottomA, -2);
		glVertex3f(leftA + a.collider.x, bottomA, -2);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(leftA, bottomA, 2);
		glVertex3f(leftA, bottomA + a.collider.y, -2);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(leftB, bottomB, -2);
		glVertex3f(leftB + b.collider.x, bottomB, -2);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(leftB, bottomB, -2);
		glVertex3f(leftB, bottomB + b.collider.y, -2);
		glEnd();
	}
	

	const bool x = leftA + a.collider.x >= leftB && leftB + b.collider.x >= leftA;

	const bool y = bottomA + a.collider.y >= bottomB && bottomB + b.collider.y >= bottomA;

	return x && y;
}

inline bool CheckCollisionX(const Object& a, const Object& b) {
	if (!a.collision || !b.collision)
		return false;

	const float leftA = a.position.x - a.collider.x / 2;
	const float leftB = b.position.x - b.collider.x / 2;

	return leftA + a.collider.x >= leftB && leftB + b.collider.x >= leftA;
}

inline bool CheckCollisionRL(const Object& a, const Object& b) {
	if (!a.collision || !b.collision)
		return false;

	const float rightA = a.position.x + a.colliderOffset.x + a.collider.x / 2;
	const float leftB = b.position.x + b.colliderOffset.x - b.collider.x / 2;
	const float bottomB = b.position.y + b.colliderOffset.y - b.collider.y / 2;
	const float bottomA = a.position.y + a.colliderOffset.y - a.collider.y / 2;

	const bool y = bottomA + a.collider.y >= bottomB && bottomB + b.collider.y >= bottomA;

	return rightA >= leftB && rightA - 0.05f <= leftB && y;
}

inline bool CheckCollisionLR(const Object& a, const Object& b) {
	if (!a.collision || !b.collision)
		return false;

	const float leftA = a.position.x - a.collider.x / 2;
	const float rightB = b.position.x + b.collider.x / 2;
	const float bottomB = b.position.y + b.colliderOffset.y - b.collider.y / 2;
	const float bottomA = a.position.y + a.colliderOffset.y - a.collider.y / 2;

	const bool y = bottomA + a.collider.y >= bottomB && bottomB + b.collider.y >= bottomA;

	return rightB >= leftA && rightB - 0.05f <= leftA && y;
}

inline bool CheckCollisionY(const Object& a, const Object& b) {
	const float bottomA = a.position.y - a.collider.y / 2;
	const float bottomB = b.position.y - b.collider.y / 2;

	return bottomA + a.collider.y >= bottomB && bottomB + b.collider.y >= bottomA;
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