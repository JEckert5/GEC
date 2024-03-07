#pragma once

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

inline struct Mesh {
	Mesh(const std::vector<float>& verts, const std::vector<GLuint>& inds) {
		vertices = verts;
		indices = inds;
	}

	Mesh() = default;

	std::vector<float> vertices{};
	std::vector<GLuint> indices{};
} Square({
		// positions         // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left
	}, {
		0, 1, 3,
		1, 2, 3,
	});

#endif
