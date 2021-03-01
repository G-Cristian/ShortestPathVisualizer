
#include "Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

/*************  CONSTS  *****************/

const double fps = 30.0;		//frames per second
const double secondsPerFrame = 1.0 / fps;

/***********  END CONSTS  ***************/

/*************  GLOBALS  ****************/

Camera camera;

/***********  END GLOBALS  **************/


/************  FUNCTIONS  ***************/
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);

void init(GLFWwindow*);
void display(GLFWwindow*, double dt);
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
	int width, height;

	glfwGetWindowSize(window, &width, &height);
	camera.resetCamera(width, height);
}

void display(GLFWwindow* window, double dt) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}