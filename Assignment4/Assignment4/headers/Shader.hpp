#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

// https://learnopengl.com/Getting-started/Shaders

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
public:
	GLuint id;
	Shader() = default;
	Shader(const char*, const char*);

	void use() const;
	static void unbind();
	void setBool(const std::string&, bool) const;
	void setInt(const std::string&, int) const;
	void setFloat(const std::string&, float) const;
	void setMat4(const std::string&, glm::mat4) const;
	void setVec3(const std::string&, glm::vec3) const;

	[[nodiscard]] bool isValid() const {
		return id != 0;
	}
private:
	static void checkCompile(unsigned int, const std::string&);
};

struct ShaderLib {
	inline static Shader Default = Shader();
	inline static Shader Text = Shader();

	static void init() {
		Default = Shader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
		Text = Shader("shaders/TextVertexShader.glsl", "shaders/TextFragmentShader.glsl");
	}
};

#endif