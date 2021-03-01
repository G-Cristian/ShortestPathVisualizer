
#include "Camera.h"
#include "Button.h"

#include <OpenGLWrapper.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <string>

/*************  CONSTS  *****************/

const double fps = 30.0;		//frames per second
const double secondsPerFrame = 1.0 / fps;

/***********  END CONSTS  ***************/

/*************  GLOBALS  ****************/

GLuint vao;
GLuint renderingProgram;

Camera camera;
std::unique_ptr<Button> btnAStar;

/***********  END GLOBALS  **************/


/************  FUNCTIONS  ***************/
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);

void init(GLFWwindow*);
void createAStarButton();
void display(GLFWwindow*, double dt);

GLuint loadImage(const std::string& file);
void createButtonRenderingProgram(const std::string& vertShaderFile, const std::string& fragShaderFile);
void drawButton(const Button& button);

/**********  END FUNCTIONS  *************/

int main() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Shortest Path Visualizer", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_reshape_callback);

	init(window);

	double lastTime = glfwGetTime();
	double currentTime = 0.0;
	double deltaTime = 0.0;
	double elapsedTimeSinceLastFrame = 0.0;

	//std::cout << "Seconds per frame: " << secondsPerFrame << std::endl;

	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		elapsedTimeSinceLastFrame += deltaTime;
		if (elapsedTimeSinceLastFrame >= secondsPerFrame) {
			//std::cout << "Elapsed seconds since last frame: " << elapsedTimeSinceLastFrame << std::endl;
			//std::cout << "FPS: " << fps*secondsPerFrame / elapsedTimeSinceLastFrame << std::endl;

			elapsedTimeSinceLastFrame -= secondsPerFrame;
			display(window, deltaTime);
			glfwSwapBuffers(window);
		}
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	camera.resetCamera(newWidth, newHeight);
}

void init(GLFWwindow* window) {
	//sets the clearing color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//generates and bind vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//creates button rendering program
	createButtonRenderingProgram("ButtonVertShader.glsl", "ButtonFragShader.glsl");


	//creates camera
	int width, height;

	glfwGetWindowSize(window, &width, &height);
	camera.resetCamera(width, height);

	//creates starButton
	createAStarButton();
}

void createAStarButton() {
	GLuint txtAStar = loadImage("A_Star.bmp");
	btnAStar = std::unique_ptr<Button>(new Button(txtAStar, 400.0f, 300.0f, 30, 30));
}

void display(GLFWwindow* window, double dt) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawButton(*btnAStar);
}

GLuint loadImage(const std::string& file) {
	int returnCode = 0;
	GLuint textureVBO = loadAndCreateTexture(file.c_str(), &returnCode);
	if (returnCode != 1) {
		std::cout << "Error loading " << file << std::endl;
	}
	else {
		std::cout << "Success loading " << file << std::endl;
	}

	std::cout << "loadImage returnCode: " << returnCode << std::endl;

	return textureVBO;
}

void createButtonRenderingProgram(const std::string& vertShaderFile, const std::string& fragShaderFile) {
	int returnCode = 0;
	std::string returnMsg;
	renderingProgram = createRenderingProgram(vertShaderFile.c_str(), fragShaderFile.c_str(), returnCode, returnMsg);

	if (returnCode == 1) {
		std::cout << "Success creating rendering program" << std::endl;
	}
	else {
		std::cout << "Error creating rendering program" << std::endl;
	}

	std::cout << "createButtonRenderingProgram returnCode: " << returnCode  << std::endl;
	std::cout << "createButtonRenderingProgram returnMsg: " << returnMsg << std::endl;
}
void drawButton(const Button& button) {
	std::string error;

	glUseProgram(renderingProgram);

	GLuint mvpMatrixLoc = glGetUniformLocation(renderingProgram, "mvpMatrix");
	
	glm::mat4 mMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(button.position().x, button.position().y, 0.0f));
	glm::mat4 vMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	glm::mat4 mvpMatrix = camera.projectionMatrix() * vMatrix * mMatrix;

	//send model view projection matrix
	glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	// send vertices
	glBindBuffer(GL_ARRAY_BUFFER, button.verticesVBO());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// send texture coords
	glBindBuffer(GL_ARRAY_BUFFER, button.textureVBO());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	//send texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, button.texture());

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}