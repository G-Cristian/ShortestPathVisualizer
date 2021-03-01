#include "Button.h"

#include <GL/glew.h>

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