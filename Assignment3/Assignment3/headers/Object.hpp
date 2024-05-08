#pragma once

#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <vector>
#include <GL/glew.h>
#include <glm/fwd.hpp>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



class Object {
public:

	enum class ObjectType {
		OBJECT,
		SPRITE,
		PLAYER,
		COLLECTABLE,
	};

	Object() = default;
	Object(std::string name, Mesh mesh, const std::vector<Texture>& textures, bool isSprite = false);
	static Object create(const std::string& name, const std::vector<Texture>& textures = {}, const Mesh& mesh = Square);
	~Object() = default;

	virtual void update();
	virtual void draw(const glm::mat4& view, const glm::mat4& proj);
	[[nodiscard]] bool isSprite() const;

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

protected:
	Mesh mMesh;
	GLuint mVAO, mVBO, mEBO;
	std::vector<Texture> mTextures;
	Texture mActiveTexture;
	glm::mat4 mTransMat;
	bool mIsSprite;
	void updateMatrix();
	void setupBuffers();
	void setDefaults();

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Sprite : public Object {
public:
	static Sprite create(const std::string& name, int columns, int baseRow, double xOffset, double yOffset, double frameTime, const std::vector<Texture>& textures);

	void update() override;

protected:
	Sprite(const std::string& name, int columns, int baseRow, double xOffset, double yOffset, double frameTime, const std::vector<Texture>& textures);

	int mColumns;
	int mCurrentRow = 0, mCurrentColumn = 0;
	int mBaseRow = 1;
	double mXOffset, mYOffset;
	int mFrame;
	double mFrameTimer, mInterval;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Player: public Sprite {
public:
	Player(float speed, float jh);

	void update() override;

	float speed;
	float jumpHeight;
	float yVel = 0.f;
	bool jumped;
	bool grounded;
	bool blockedLeft, blockedRight;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Coin: public Sprite {
public:
	Coin();

	void update() override;
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