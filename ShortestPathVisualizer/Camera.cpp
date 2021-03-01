#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Camera::resetCamera(int newWidth, int newHeight) {
	_width = newWidth;
	_height = newHeight;

	_projectionMatrix = glm::ortho(0.0f, static_cast<float>(_width), 0.0f, static_cast<float>(_height), 0.1f, 100.0f);
}