#include "Text.hpp"

Text::Text(std::string str, glm::vec2 pos, glm::vec3 color, Shader* shader, float scale):
	text(std::move(str)),
	position(pos),
	scale(scale),
	color(color),
	mShader(shader)
{
	mProj = glm::ortho(0.f, 600.f, 0.f, 600.f);
	mIndices = { 0, 1, 2, 0, 2, 3 };
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mIndices.size(), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::draw() {
	mShader->use();
	mShader->setVec3("textColor", color);
	mShader->setMat4("projection", mProj);

	glBindVertexArray(mVAO);

	float x = position.x;

	for (auto c = text.cbegin(); c != text.cend(); ++c) {
		Char ch = Char::Chars[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float verts[4][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

		//	{xpos, ypos + h, 0.0f, 0.0f},
		//	{xpos + w, ypos, 1.0f, 1.0f},
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texID);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * mIndices.size(), mIndices.data());

		// glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mShader->disable();
}
