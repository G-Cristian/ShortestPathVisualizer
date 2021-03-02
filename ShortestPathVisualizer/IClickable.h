#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IClickable {
public:
	virtual ~IClickable() = default;

	virtual void onCLick(GLFWwindow* window, float x, float y) = 0;
protected:
	IClickable() = default;
};