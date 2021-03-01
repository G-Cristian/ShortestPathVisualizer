#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	const glm::mat4& projectionMatrix() const { return _projectionMatrix; }
	glm::mat4& projectionMatrix() { return _projectionMatrix; }

	float aspect() {
		return static_cast<float>(_width) / static_cast<float>(_height);
	}

	void resetCamera(int newWidth, int newHeight);

private:
	int _width;
	int _height;
	glm::mat4 _projectionMatrix;
};

