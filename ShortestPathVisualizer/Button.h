#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Button
{
public:
	Button(GLuint texture, float x, float y, float width = 10.0f, float height = 10.0f) :
		_texture(texture),
		_verticesVBO(0),
		_textureVBO(0),
		_position(x, y),
		_size(width, height),
		_index(-1){

		createButton();
	}

	~Button() = default;

	const GLuint& texture() const { return _texture; }
	GLuint& texture() { return _texture; }

	const GLuint& verticesVBO() const { return _verticesVBO; }

	const GLuint& textureVBO() const { return _textureVBO; }

	const glm::vec2& position() const { return _position; }
	glm::vec2& position() { return _position; }

	const glm::vec2& size() const { return _size; }
	//glm::vec2& size() { return _size; }

	const int& index() const { return _index; }
	int& index() { return _index; }

private:
	void createButton();

	GLuint _texture;
	GLuint _verticesVBO;
	GLuint _textureVBO;

	glm::vec2 _position;
	glm::vec2 _size;

	int _index;
};

