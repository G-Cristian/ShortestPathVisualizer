#include "Grid.h"

#include "Button.h"
#include "ButtonClickNotifier.h"
#include "IClickable.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

void Grid::addClickListener(onButtonClick observer) {
	_buttonClickNotifier.addObserver(observer);
}

void Grid::onCLick(GLFWwindow* window, float x, float y) {
	int windowWidth = 0;
	int windowHeight = 0;
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	glm::vec2 size_2 = _size / 2.0f;

	float left = _center.x * static_cast<float>(windowWidth) - size_2.x;
	float right = _center.x * static_cast<float>(windowWidth) + size_2.x + (2.0f*(float)(_xButtons));
	float top = _center.y * static_cast<float>(windowHeight) - size_2.y;
	float bottom = _center.y * static_cast<float>(windowHeight) + size_2.y + (2.0f * (float)(_yButtons));

	if (x >= left && x <= right && y >= top && y <= bottom) {
		Button* clickedButton = getClickedButton(x-left, y-top);
		
		_buttonClickNotifier.notify(clickedButton);
	}
}

void Grid::init(GLuint buttonTexture) {
	glm::vec2 buttonSize = getButtonSize();
	glm::vec2 buttonSize_2 = buttonSize / 2.0f;

	_buttons = std::vector<std::vector<Button>>();
	_buttons.reserve(_yButtons);

	for (int y = 0; y < _yButtons; ++y) {
		_buttons.push_back(std::vector<Button>());
		_buttons[y].reserve(_xButtons);

		for (int x = 0; x < _xButtons; ++x) {
			Button button(buttonTexture,
				(buttonSize_2.x + (2.0f + buttonSize.x) * (float)x) / _size.x,
				(buttonSize_2.y + (2.0f + buttonSize.y) * (float)y) / _size.y,
				buttonSize.x,
				buttonSize.y);

			button.index() = x + y * _xButtons;

			_buttons[y].push_back(button);
		}
	}
}

Button* Grid::getClickedButton(float xRelativeFromLeft, float yRelativeFromTop){
	glm::vec2 buttonSize = getButtonSize();

	int x = static_cast<int>(xRelativeFromLeft / (buttonSize.x + 2.0f));
	int y = static_cast<int>(yRelativeFromTop / (buttonSize.y + 2.0f));

	return &(_buttons[y][x]);
}

glm::vec2 Grid::getButtonSize() const {
	return glm::vec2(_size.x / static_cast<float>(_xButtons), _size.y / static_cast<float>(_yButtons));
}