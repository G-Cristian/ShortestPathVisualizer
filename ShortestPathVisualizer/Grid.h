#pragma once

#include "Button.h"
#include "ButtonClickNotifier.h"
#include "IClickable.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Grid:public IClickable
{
public:
	Grid(float centerX, float centerY, float width, float height, int xButtons, int yButtons, GLuint buttonTexture) :
		_center(centerX, centerY),
		_size(width, height),
		_xButtons(xButtons),
		_yButtons(yButtons){

		init(buttonTexture);
	}

	~Grid() = default;

	const glm::vec2& center() const { return _center; }

	const glm::vec2& size() const { return _size; }
	
	int xButtons()const { return _xButtons; }
	int yButtons() const { return _yButtons; }

	const std::vector<std::vector<Button>>& buttons() const { return _buttons; }

	//onButtonClick notifier methods
	void addClickListener(onButtonClick);

	//override methods
	void onCLick(GLFWwindow* window, float x, float y) override final;

private:
	void init(GLuint buttonTexture);
	Button* getClickedButton(float xRelativeFromLeft, float yRelativeFromTop);
	glm::vec2 getButtonSize() const;

	glm::vec2 _center;
	glm::vec2 _size;
	int _xButtons;
	int _yButtons;

	std::vector<std::vector<Button>> _buttons;

	ButtonClickNotifier _buttonClickNotifier;
};

