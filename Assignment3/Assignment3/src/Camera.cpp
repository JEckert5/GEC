#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>

Camera Camera::fromOrtho(float rl, float tb, float z) {
	Camera c;

	c.type = ProjType::ORTHOGRAPHIC;
	c.mProjection = glm::ortho(-rl, rl, -tb, tb, -z, z);
	c.view = glm::identity<glm::mat4>();

	return c;
}

Camera Camera::fromPerspective(float fov, float aspect, float n, float f) {
	Camera c;

	c.type = ProjType::PERSPECTIVE;
	c.fov = fov;
	c.mProjection = glm::perspective(c.fov, aspect, n, f);

	return c;
}

void Camera::update() {
	view = glm::identity<glm::mat4>();

	view = translate(view, -position);
	view = rotate(view, rotation.x, { 1, 0, 0 });
	view = rotate(view, rotation.y, { 0, 1, 0 });
	view = rotate(view, rotation.z, { 0, 0, 1 });
}

glm::mat4 Camera::getProjection() const {
	return mProjection;
}

Camera::Camera(): type(ProjType::UNINIT), fov(0), position(0), rotation(0), view(glm::identity<glm::mat4>()), mProjection(glm::identity<glm::mat4>()) {
	
}

