#include "Text.hpp"

#include "Config.hpp"

#include <glm/ext/matrix_clip_space.hpp>

Text::Text(std::string str, const glm::vec2 pos, const glm::vec3 color, const float scale, const glm::mat4& proj):
	str(std::move(str)),
	position(pos),
	scale(scale),
	color(color),
	mVAO(0),
	mVBO(0),
	mEBO(0),
	mProj(proj)
{ 
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::draw() const {
	if (!active)
		return;

	ShaderLib::Text.use();
	ShaderLib::Text.setVec3("textColor", color);
	ShaderLib::Text.setMat4("projection", mProj);

	glBindVertexArray(mVAO);

	float x = position.x;
	float y = position.y;

	for (auto c = str.cbegin(); c != str.cend(); ++c) {
		Char ch = Char::Chars[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float verts[4][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texID);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * mIndices.size(), mIndices.data());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Shader::unbind();
}

void Text::setProjection(const glm::mat4& newProj) {
	mProj = newProj;
}
