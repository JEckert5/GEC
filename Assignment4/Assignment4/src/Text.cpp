#include "Text.hpp"

#include "Config.hpp"

#include <glm/ext/matrix_clip_space.hpp>

Text::Text(std::string str, std::string font, const glm::vec2 pos, const glm::vec3 color, const float scale, const glm::mat4& proj):
	str(std::move(str)),
	font(std::move(font)),
	position(pos),
	scale(scale),
	color(color),
	m_vao(0),
	m_vbo(0),
	m_ebo(0),
	m_projection(proj)
{ 
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), nullptr, GL_DYNAMIC_DRAW);

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
	ShaderLib::Text.setMat4("projection", m_projection);

	glBindVertexArray(m_vao);

	float x = position.x;
	float y = position.y;

	for (auto c = str.cbegin(); c != str.cend(); ++c) {
		Char ch = Char::Fonts[font][*c];
		// Char ch = Char::Chars[*c];

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

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * m_indices.size(), m_indices.data());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Shader::unbind();
}

void Text::setProjection(const glm::mat4& newProj) {
	m_projection = newProj;
}
