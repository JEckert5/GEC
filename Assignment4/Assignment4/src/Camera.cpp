#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>

glm::vec3 XAxis = { 1, 0, 0 };
glm::vec3 YAxis = { 0, 1, 0 };
glm::vec3 ZAxis = { 0, 0, 1 };

Camera Camera::fromOrtho(float rl, float tb, float z) {
	Camera c;

	c.type = ProjType::ORTHOGRAPHIC;
	c.m_projection = glm::ortho(-rl, rl, -tb, tb, -z, z);
	c.view = glm::identity<glm::mat4>();

	return c;
}

Camera Camera::fromPerspective(float fov, float aspect, float n, float f) {
	Camera c;

	c.type = ProjType::PERSPECTIVE;
	c.fov = fov;
	c.m_projection = glm::perspective(c.fov, aspect, n, f);

	return c;
}

void Camera::update() {
	view = glm::identity<glm::mat4>();

	view = translate(view, -position);
	view = rotate(view, rotation.x, XAxis);
	view = rotate(view, rotation.y, YAxis);
	view = rotate(view, rotation.z, ZAxis);
	view = scale(view, glm::vec3(zoom));
}

glm::mat4 Camera::getProjection() const {
	return m_projection;
}

Camera::Camera(): type(ProjType::UNINIT), fov(0), position(0), rotation(0), view(glm::identity<glm::mat4>()), m_projection(glm::identity<glm::mat4>()) {
	
}

