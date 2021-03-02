#include "Button.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Button::createButton() {
	float width_2 = _size.x/2.0f;
	float height_2 = _size.y / 2.0f;

	float vertices[18] = {
		width_2, height_2, 0.0f, -width_2, height_2, 0.0f, -width_2, -height_2, 0.0f,
		width_2, height_2, 0.0f, -width_2, -height_2, 0.0f, width_2, -height_2, 0.0f
	};

	glGenBuffers(1, &_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);

	float texture[12] = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
	};

	glGenBuffers(1, &_textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture), (void*)texture, GL_STATIC_DRAW);
}

//onButtonClick notifier methods
void Button::addClickListener(onButtonClick listener) {
	_buttonClickNotifier.addObserver(listener);
}

//override methods
void Button::onCLick(GLFWwindow* window, float x, float y) {
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(window, &width, &height);

	glm::vec2 size_2 = _size / 2.0f;

	float left = width * _position.x - size_2.x;
	float right = width * _position.x + size_2.x;
	float top = height * _position.y - size_2.y;
	float bottom = height * _position.y + size_2.y;

	if (x >= left && x <= right && y >= top && y <= bottom) {
		_buttonClickNotifier.notify(this);
	}
}