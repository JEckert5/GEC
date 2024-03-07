#include "Shader.hpp"

Shader::Shader() {
	mID = 0;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::ifstream vertexFile(vertexPath);
	std::ifstream fragmentFile(fragmentPath);
	std::stringstream vStream, fStream;

	vStream << vertexFile.rdbuf();
	fStream << fragmentFile.rdbuf();

	vertexFile.close();
	fragmentFile.close();

	const std::string vertexCode = vStream.str();
	const std::string fragmentCode = fStream.str();

	unsigned int v, f;
	const char* vPain = vertexCode.c_str();
	const char* fPain = fragmentCode.c_str();

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(v, 1, &vPain, nullptr);
	glCompileShader(v);
	checkCompile(v, "VERTEX");

	glShaderSource(f, 1, &fPain, nullptr);
	glCompileShader(f);
	checkCompile(f, "FRAGMENT");

	mID = glCreateProgram();
	glAttachShader(mID, v);
	glAttachShader(mID, f);
	glLinkProgram(mID);
	checkCompile(mID, "PROGRAM");

	glDeleteShader(v);
	glDeleteShader(f);
}

void Shader::use() const {
	glUseProgram(mID);
}

void Shader::setBool(const std::string& name, const bool value) const {
	glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, const int value) const {
	glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}


void Shader::setFloat(const std::string& name, const float value) const {
	glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompile(const unsigned int shader, const std::string& type) const {
	int succ;
	char log[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &succ);

		if (!succ) {
			glGetShaderInfoLog(shader, 1024, nullptr, log);
			std::cout << "Shader compilation error. Type: " << type << "\n" << log << "\n";
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &succ);

		if (!succ) {
			glGetProgramInfoLog(shader, 1024, nullptr, log);
			std::cout << "Program linking error. Type: " << type << "\n" << log << "\n";
		}
	}
}
