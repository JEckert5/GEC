#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera {
public:
	enum class ProjType {
		PERSPECTIVE,
		ORTHOGRAPHIC,
		UNINIT
	};

	Camera();

	static Camera fromOrtho(float rl, float tb, float z);
	static Camera fromPerspective(float fov, float aspect, float n, float f);

	void update();
	[[nodiscard]] glm::mat4 getProjection() const;

	ProjType type;
	float fov;
	glm::vec3 position;
	glm::vec3 rotation;
	float zoom = 1.0f;
	glm::mat4 view;

private:
	glm::mat4 m_projection;
};

