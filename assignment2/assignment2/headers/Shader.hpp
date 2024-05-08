#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

// https://learnopengl.com/Getting-started/Shaders

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	GLuint id;
	Shader();
	Shader(const char*, const char*);

	void use() const;
	void disable() const;
	void setBool(const std::string&, bool) const;
	void setInt(const std::string&, int) const;
	void setFloat(const std::string&, float) const;
	void setMat4(const std::string&, glm::mat4) const;
	void setVec3(const std::string&, glm::vec3) const;
private:
	void checkCompile(unsigned int, const std::string&) const;
};

#endif