#include "pch.h"
#include <OpenGLWrapper.h>

#include <fstream>
#include <GL/glew.h>
#include <sstream>
#include <string>
#include <SOIL2/SOIL2.h>

int getOpenGLError(int* errorsBuffer, int bufferSize, bool* moreErrors) {
	int* errorsBufferCopy = errorsBuffer;
	int numberOfErrors = 0;
	*moreErrors = true;
	while (*moreErrors && errorsBufferCopy != (errorsBuffer + bufferSize)) {
		int glError = glGetError();
		if (glError == GL_NO_ERROR) {
			*moreErrors = false;
		}
		else {
			++numberOfErrors;
			*errorsBufferCopy = glError;
			++errorsBufferCopy;
		}
	}

	return numberOfErrors;
}

std::string __stdcall getOpenGLError(int numberOfErrorsToShow) {
	std::stringstream ss;
	int remainingErrorsToShow = numberOfErrorsToShow;
	int glError = 0;
	while (numberOfErrorsToShow == 0 || remainingErrorsToShow-- > 0) {
		glError = glGetError();
		if (glError == GL_NO_ERROR) {
			break;
		}
		ss << "glError: " << glError << std::endl;
	}

	return ss.str();
}

int getShaderLog(GLuint shader, char* logBuffers, int logSize) {
	int retVal = 0;
	GLint len = 0;
	GLsizei chWrittn = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len >= logSize) {
		// cannot fit log in an element of logBuffers
		retVal = -1;
	}
	else if (len > 0) {
		// set the log inside the corresponding logBuffersCopy elements
		// and go to next position
		glGetShaderInfoLog(shader, len, &chWrittn, logBuffers);
		retVal = len;
	}

	return retVal;
}

std::string __stdcall getShaderLog(GLuint shader) {
	GLint len;
	GLsizei chWrittn;
	char* log;

	std::stringstream ss;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		ss << "Shader Info Log: " << log << std::endl;
		free(log);
	}

	return ss.str();
}

std::string __stdcall getProgramLog(GLuint program) {
	GLint len;
	GLsizei chWrittn;
	char* log;
	
	std::stringstream ss;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(program, len, &chWrittn, log);
		ss << "Program Info Log: " << log << std::endl;
		free(log);
	}
	
	return ss.str();
}

std::string __stdcall readShaderSource(const char* shaderSourcePath, int& returnCode) {
	returnCode = 1;
	std::string line;
	std::stringstream ss;

	std::ifstream file(shaderSourcePath, std::ios::in);
	if (!file.is_open()) {
		returnCode = -1;

		return "File not opened";
	}

	while (!file.eof()) {
		std::getline(file, line);
		ss << line << std::endl;
	}

	file.close();

	return ss.str();
}

GLuint __stdcall createAndCompileShader(const char* shaderPath, GLenum shaderType, int& returnCode, std::string& returnMsg) {
	returnCode = 1;
	returnMsg = "";

	std::string shaderStr = readShaderSource(shaderPath, returnCode);
	if (returnCode != 1) {
		std::stringstream ss;
		ss << "Error reading shader '" << shaderPath << "'" << std::endl;
		ss << shaderStr << std::endl;
		returnMsg = ss.str();

		return 0;
	}

	GLuint shader = glCreateShader(shaderType);
	const char* shaderContent = shaderStr.c_str();
	glShaderSource(shader, 1, &shaderContent, NULL);
	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1) {
		returnCode = -3;
		std::stringstream ss;
		ss << "Shader compilation failed for '" << shaderPath << "'" << std::endl;
		ss << getShaderLog(shader) << std::endl;
		returnMsg = ss.str();
	}

	return shader;
}

GLuint __stdcall createAndCompileVertexShader(const char* shaderPath, int& returnCode, std::string& returnMsg) {
	return createAndCompileShader(shaderPath, GL_VERTEX_SHADER, returnCode, returnMsg);
}

GLuint __stdcall createAndCompileFragmentShader(const char* shaderPath, int& returnCode, std::string& returnMsg) {
	return createAndCompileShader(shaderPath, GL_FRAGMENT_SHADER, returnCode, returnMsg);
}

GLuint __stdcall createRenderingProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath, int& returnCode, std::string& returnMsg) {
	returnCode = 1;
	returnMsg = "";

	GLuint vertexShader = createAndCompileVertexShader(vertexShaderFilePath, returnCode, returnMsg);

	if (returnCode != 1) {
		return 0;
	}

	GLuint fragmentShader = createAndCompileFragmentShader(fragmentShaderFilePath, returnCode, returnMsg);
	if (returnCode != 1) {
		return 0;
	}

	GLuint renderingProgram = glCreateProgram();
	GLint linked = 0;
	glAttachShader(renderingProgram, vertexShader);
	glAttachShader(renderingProgram, fragmentShader);

	glLinkProgram(renderingProgram);
	glGetProgramiv(renderingProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		returnCode = -2;
		std::stringstream ss;
		ss << "Program link failed" << std::endl;
		ss << getProgramLog(renderingProgram);
		returnMsg = ss.str();
	}

	return renderingProgram;
}

int initGlew() {
	return glewInit();
}

void clearColorBuffer() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void clearDepthBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void enableGLAttribute(GLenum attribute) {
	glEnable(attribute);
}

void enableCullFace() {
	glEnable(GL_CULL_FACE);
}

void enableDepthTest() {
	glEnable(GL_DEPTH_TEST);
}

void setDepthFunction(GLenum depthFunction) {
	glDepthFunc(depthFunction);
}

void setDepthFunctionLessEqual() {
	glDepthFunc(GL_LEQUAL);
}

void setFrontFace(GLenum frontFace) {
	glFrontFace(frontFace);
}

void setFrontFaceClockwise() {
	glFrontFace(GL_CW);
}

void setFrontFaceCounterClockwise() {
	glFrontFace(GL_CCW);
}

GLuint createAndBindVAO() {
	GLuint vao = 0;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return vao;
}

GLuint createArrayBuffer(const float* attributes, GLsizeiptr numberOfAttributes) {
	GLuint vbo = 0;
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numberOfAttributes * sizeof(float), (const void*)attributes, GL_STATIC_DRAW);

	return vbo;
}

GLuint createElementArrayBuffer(const int* attributes, GLsizeiptr numberOfAttributes) {
	GLuint vbo = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfAttributes * sizeof(int), (const void*)attributes, GL_STATIC_DRAW);

	return vbo;
}

GLuint loadAndCreateTexture(const char* textureImagePath, int* returnCode) {
	GLuint textureID = 0;
	*returnCode = 1;
	textureID = SOIL_load_OGL_texture(textureImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) {
		*returnCode = -1;
	}

	return textureID;
}

GLint getUniformLocation(GLuint renderingProgram, const char* attributeName) {
	return glGetUniformLocation(renderingProgram, attributeName);
}

void setUniformMatrix4fv(GLint matrixLocation, GLfloat* matrix) {
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
}

void programUniform1i(GLuint renderingProgram, GLint location, GLint value) {
	glProgramUniform1i(renderingProgram, location, value);
}

void programUniform1f(GLuint renderingProgram, GLint location, GLfloat value) {
	glProgramUniform1f(renderingProgram, location, value);
}

void programUniform2fv(GLuint renderingProgram, GLint location, const GLfloat* value) {
	glProgramUniform2fv(renderingProgram, location, 1, value);
}

void programUniform3fv(GLuint renderingProgram, GLint location, const GLfloat* value) {
	glProgramUniform3fv(renderingProgram, location, 1, value);
}

void programUniform4fv(GLuint renderingProgram, GLint location, const GLfloat* value) {
	glProgramUniform4fv(renderingProgram, location, 1, value);
}

void activateArrayBuffer2f(GLuint index, GLuint vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(index);
}

void activateArrayBuffer3f(GLuint index, GLuint vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(index);
}

void activateTexture2D(GLenum texture, GLuint textureVBO) {
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, textureVBO);
}

void activateTexture02D(GLuint textureVBO) {
	activateTexture2D(GL_TEXTURE0, textureVBO);
}

void drawTrianglesUsingVertices(GLsizei numberOfVertices) {
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
}

void drawTrianglesUsingIndices(GLuint indices, GLsizei numberOfIndices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, NULL);
}
