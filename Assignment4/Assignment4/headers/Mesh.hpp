#pragma once

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct Mesh {
	Mesh() = default;

	Mesh(const std::vector<Vertex>& verts, const std::vector<GLuint>& inds, bool sprite = false): vbo(0), ebo(0), vao(0), dynamic(sprite), vertices(verts), indices(inds) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
		glEnableVertexAttribArray(1);
	}

	void draw() const {
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	bool dynamic;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

// need to delay using the mesh constructor until after GLUT is initialized.
struct DefaultMesh {
	inline static Mesh Square{};
	inline static Mesh SpriteSquare{};

	static void initDefaultMesh() {
		Square = Mesh({ {{0.25f,  0.25f, 0.0f}, {1, 1}},
		{{0.25f, -0.25f, 0.0f}, { 1.0f, 0.0f}},
		{{-0.25f, -0.25f, 0.0f}, {0.0f, 0.0f}},
		{{-0.25f,  0.25f, 0.0f}, {0.0f, 1.0f}} }, {
			0, 1, 3,
			1, 2, 3
		});

		SpriteSquare = Mesh({ {{0.25f,  0.25f, 0.0f}, {1, 1}},
		{{0.25f, -0.25f, 0.0f}, { 1.0f, 0.0f}},
		{{-0.25f, -0.25f, 0.0f}, {0.0f, 0.0f}},
		{{-0.25f,  0.25f, 0.0f}, {0.0f, 1.0f}} }, {
			0, 1, 3,
			1, 2, 3
		}, true);
	}
};

#endif
