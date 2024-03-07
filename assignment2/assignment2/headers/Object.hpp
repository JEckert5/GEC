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
	Object(const Mesh& mesh, Shader* shader, const std::string& texPath);

	~Object();

	virtual void draw();
	virtual void update(const int deltaTime, const int elapsedTime);
	Shader* getShader() const;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

protected:
	GLuint mVAO{}, mVBO{}, mEBO{};
	Mesh mMesh;
	glm::mat4 mTransMat;
	Shader* mShader;
	Texture mTexture;
};

#endif
