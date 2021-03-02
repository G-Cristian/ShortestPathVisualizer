#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

class IClickable;

class WindowClickNotifier
{
public:
	WindowClickNotifier(GLFWwindow* window) :
		_window(window) {
	}

	~WindowClickNotifier() = default;

	void addObserver(std::shared_ptr<IClickable>);
	//I will not remove observers in this application
	//void removeObserver(onButtonClick);

	void notify(float x, float y);
private:
	GLFWwindow* _window;
	std::vector<std::shared_ptr<IClickable>> _observers;
};

