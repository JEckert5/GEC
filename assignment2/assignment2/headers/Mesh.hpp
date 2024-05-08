#pragma once

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

inline struct Mesh {
	Mesh(const std::vector<float>& verts, const std::vector<GLuint>& inds) {
		for (int i = 0; i < verts.size(); i += 5) {
			Vertex v{{}, {}};

			v.position = glm::vec3(verts[0 + i], verts[1 + i], verts[2 + i]);
			v.texCoords = glm::vec2(verts[3 + i], verts[4 + i]);

			vertices.emplace_back(v);
		}

		indices = inds;
	}

	Mesh(const std::vector<Vertex>& verts, const std::vector<GLuint>& inds) {
		vertices = verts;
		indices = inds;
	}

	Mesh() = default;

	std::vector<Vertex> vertices{};
	std::vector<GLuint> indices{};
}Square(
{{{0.25f,  0.25f, 0.0f}, {1, 1}}, 
	{{0.25f, -0.25f, 0.0f}, { 1.0f, 0.0f}},
	{{-0.25f, -0.25f, 0.0f}, {0.0f, 0.0f}},
	{{-0.25f,  0.25f, 0.0f}, {0.0f, 1.0f}}},
{
	0, 1, 3,
	1, 2, 3
}
);

#endif
