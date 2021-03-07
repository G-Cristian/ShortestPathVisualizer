//first add OpenGL includes
#include <GL/glew.h>
#include <OpenGLWrapper.h>

#include "AStar.h"
#include "Button.h"
#include "Camera.h"
#include "Dijkstra.h"
#include "Graph.h"
#include "Grid.h"
#include "IShortestPathStrategy.h"
#include "WindowClickNotifier.h"


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

/**************  ENUMS  *****************/

enum SelectingMode
{
	ClearTile = 0,
	StartTile = 1,
	EndTile = 2,
	BlockTile = 3
};

/************  END ENUMS  ***************/

/*************  CONSTS  *****************/

const double fps = 30.0;		//frames per second
const double secondsPerFrame = 1.0 / fps;

const int gridXButtons = 28;
const int gridYButtons = 17;

const double secondsPerUpdate = 0.1f;

/***********  END CONSTS  ***************/

/*************  GLOBALS  ****************/

std::unique_ptr<WindowClickNotifier> windowClickNotifier;

GLuint vao;
GLuint renderingProgram;

Camera camera;
std::shared_ptr<Button> btnAStar;
std::shared_ptr<Button> btnDijkstra;

std::shared_ptr<Button> btnClear;
std::shared_ptr<Button> btnStart;
std::shared_ptr<Button> btnEnd;
std::shared_ptr<Button> btnBlock;

std::shared_ptr<Button> btnBegin;

std::shared_ptr<Grid> grid;

GLuint whiteTexture = 0;
GLuint whiteClickedTexture = 0;
GLuint startTexture = 0;
GLuint startClickedTexture = 0;
GLuint endTexture = 0;
GLuint endClickedTexture = 0;
GLuint blockTexture = 0;
GLuint blockClickedTexture = 0;

GLuint beginTexture = 0;
GLuint beginClickedTexture = 0;

GLuint dijkstraNotClickedTexture = 0;
GLuint dijkstraClickedTexture = 0;

GLuint aStarNotClickedTexture = 0;
GLuint aStarClickedTexture = 0;

bool dijkstra = true;
bool executing = false;
bool isThereStartButton = false;
bool isThereEndButton = false;

double elapsedTimeSinceLastUpdate = 0.0;

SelectingMode selectingMode = SelectingMode::ClearTile;

std::vector<std::vector<SelectingMode>> statusGrid;
std::shared_ptr<Graph> graph;
std::unique_ptr<IShortestPathStrategy> shortestPathStrategy;

/***********  END GLOBALS  **************/


/************  FUNCTIONS  ***************/
void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void init(GLFWwindow*);
void initStatusGrid();
void resetGridTextures();
void createTextures();
void createGrid();
void createDijkstraButton();
void createAStarButton();
void createClearButton();
void createStartButton();
void createEndButton();
void createBlockButton();
void createBeginButton();

void update(GLFWwindow*, double dt);
void display(GLFWwindow*, double dt);

GLuint loadImage(const std::string& file);
void createButtonRenderingProgram(const std::string& vertShaderFile, const std::string& fragShaderFile);
void drawButton(const Button& button, GLFWwindow* window, float x, float y);
void drawGrid(const Grid& grid, GLFWwindow* window);

void getButtonXYFromIndex(int index, int& x, int& y);
int getIndexFromXY(int x, int y);

void initializeGraph();
int getTopAdjacentButtonIndex(int x, int y);
int getBottomAdjacentButtonIndex(int x, int y);
int getLeftAdjacentButtonIndex(int x, int y);
int getRightAdjacentButtonIndex(int x, int y);
void drawPath(const std::vector<int>& parents);

/**********  END FUNCTIONS  *************/

/*************  EVENTS  *****************/

void onDijkstraClick(Button*);
void onAStarClick(Button*);
void onGridClicked(Button*);

void onClearClick(Button*);
void onStartClick(Button*);
void onEndClick(Button*);
void onBlockClick(Button*);

void onBeginClick(Button*);

/***********  END EVENTS  ***************/

int main() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(1000, 600, "Shortest Path Visualizer", NULL, NULL);

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

		elapsedTimeSinceLastUpdate += deltaTime;
		if (elapsedTimeSinceLastUpdate >= secondsPerUpdate) {
			elapsedTimeSinceLastUpdate -= secondsPerUpdate;

			update(window, deltaTime);
		}

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double x = 0.0, y = 0.0;
		glfwGetCursorPos(window, &x, &y);
		windowClickNotifier->notify(x,y);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (!executing) {
			initStatusGrid();
			resetGridTextures();
		}
	}
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

	glfwGetFramebufferSize(window, &width, &height);
	camera.resetCamera(width, height);

	windowClickNotifier = std::unique_ptr<WindowClickNotifier>(new WindowClickNotifier(window));

	//init status grid
	selectingMode = SelectingMode::ClearTile;
	initStatusGrid();

	//load textures
	createTextures();

	//create grid
	createGrid();

	//creates buttons
	dijkstra = true;
	createDijkstraButton();
	createAStarButton();

	createClearButton();
	createStartButton();
	createEndButton();
	createBlockButton();

	executing = false;
	createBeginButton();


	//after creating everything set the mouse button callback in order to listen to clicks
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void initStatusGrid() {
	isThereStartButton = false;
	isThereEndButton = false;
	statusGrid = std::vector<std::vector<SelectingMode>>(gridYButtons, std::vector<SelectingMode>(gridXButtons, SelectingMode::ClearTile));
}

void resetGridTextures() {
	grid->resetTextures(whiteTexture);
}

void createTextures() {
	whiteTexture = loadImage("White.bmp");
	whiteClickedTexture = loadImage("White_sel.bmp");

	startTexture = loadImage("Start.bmp");
	startClickedTexture = loadImage("Start_sel.bmp");

	endTexture = loadImage("End.bmp");
	endClickedTexture = loadImage("End_sel.bmp");

	blockTexture = loadImage("Block.bmp");
	blockClickedTexture = loadImage("Block_sel.bmp");

	beginTexture = loadImage("Begin.bmp");
	beginClickedTexture = loadImage("Begin_sel.bmp");

	dijkstraNotClickedTexture = loadImage("D.bmp");
	dijkstraClickedTexture = loadImage("D_sel.bmp");

	aStarNotClickedTexture = loadImage("A_Star.bmp");
	aStarClickedTexture = loadImage("A_Star_sel.bmp");
}

void createGrid() {
	grid = std::make_shared<Grid>(0.47f, 0.5f, 840.0f, 510.0f, gridXButtons, gridYButtons, whiteTexture);
	windowClickNotifier->addObserver(grid);
	grid->addClickListener(onGridClicked);
}

void createDijkstraButton() {
	btnDijkstra = std::make_shared<Button>(dijkstraClickedTexture, 0.5f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnDijkstra);
	btnDijkstra->addClickListener(onDijkstraClick);
}

void createAStarButton() {
	btnAStar = std::make_shared<Button>(aStarNotClickedTexture, 0.55f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnAStar);
	btnAStar->addClickListener(onAStarClick);
}

void createClearButton() {
	btnClear = std::make_shared<Button>(whiteClickedTexture, 0.1f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnClear);
	btnClear->addClickListener(onClearClick);
}

void createStartButton() {
	btnStart = std::make_shared<Button>(startTexture, 0.15f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnStart);
	btnStart->addClickListener(onStartClick);
}

void createEndButton() {
	btnEnd = std::make_shared<Button>(endTexture, 0.2f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnEnd);
	btnEnd->addClickListener(onEndClick);
}

void createBlockButton() {
	btnBlock = std::make_shared<Button>(blockTexture, 0.25f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnBlock);
	btnBlock->addClickListener(onBlockClick);
}

void createBeginButton() {
	btnBegin = std::make_shared<Button>(beginTexture, 0.7f, 0.03f, 30.0f, 30.0f);
	windowClickNotifier->addObserver(btnBegin);
	btnBegin->addClickListener(onBeginClick);
}

void update(GLFWwindow*, double dt) {
	if (executing && shortestPathStrategy != nullptr) {
		int step = shortestPathStrategy->step();
		if (step == -1) {
			drawPath(shortestPathStrategy->parents());
			shortestPathStrategy = nullptr;
			executing = false;
			btnBegin->texture() = beginTexture;

			std::cout << "End" << std::endl;
		}
		else {
			int x = 0;
			int y = 0;
			getButtonXYFromIndex(step, x, y);
			grid->buttons()[y][x].texture() = startClickedTexture;
		}
	}
}

void display(GLFWwindow* window, double dt) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawButton(*btnClear, window, btnClear->position().x, btnClear->position().y);
	drawButton(*btnStart, window, btnStart->position().x, btnStart->position().y);
	drawButton(*btnEnd, window, btnEnd->position().x, btnEnd->position().y);
	drawButton(*btnBlock, window, btnBlock->position().x, btnBlock->position().y);

	drawButton(*btnDijkstra, window, btnDijkstra->position().x, btnDijkstra->position().y);
	drawButton(*btnAStar, window, btnAStar->position().x, btnAStar->position().y);

	drawButton(*btnBegin, window, btnBegin->position().x, btnBegin->position().y);

	drawGrid(*grid, window);
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

void drawButton(const Button& button, GLFWwindow* window, float x, float y) {
	std::string error;

	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);

	glUseProgram(renderingProgram);

	GLuint mvpMatrixLoc = glGetUniformLocation(renderingProgram, "mvpMatrix");
	
	glm::mat4 mMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(	x * static_cast<float>(width),
																	y * static_cast<float>(height),
																	0.0f
																 )
									  );
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

void drawGrid(const Grid& grid, GLFWwindow* window) {
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	
	float centerXPos = grid.center().x * static_cast<float>(width);
	float centerYPos = grid.center().y * static_cast<float>(height);

	float gridLeft = (centerXPos - (grid.size().x / 2.0f));
	float gridTop = (centerYPos - (grid.size().y / 2.0f));

	const auto & buttons = grid.buttons();
	for (const auto& row : buttons) {
		for (const auto& button : row) {
			float x = (gridLeft + button.position().x * grid.size().x) / static_cast<float>(width);
			float y = (gridTop + button.position().y * grid.size().y) / static_cast<float>(height);
			drawButton(button, window, x, y);
		}
	}
}

void onDijkstraClick(Button* button) {
	std::cout << "Dijkstra clicked" << std::endl;
	if (!dijkstra && !executing) {
		button->texture() = dijkstraClickedTexture;
		btnAStar->texture() = aStarNotClickedTexture;
	}

	dijkstra = true;
}

void onAStarClick(Button* button) {
	std::cout << "A* clicked" << std::endl;
	if (dijkstra && !executing) {
		button->texture() = aStarClickedTexture;
		btnDijkstra->texture() = dijkstraNotClickedTexture;
	}

	dijkstra = false;
}

void onGridClicked(Button* button) {
	std::cout << "Grid clicked. Button index: " << button->index() << std::endl;

	int x = 0;
	int y = 0;

	getButtonXYFromIndex(button->index(), x, y);

	std::cout << "Button in grid clicked x: " << x << " y: " << y << std::endl;

	if (!executing) {
		if (selectingMode == SelectingMode::StartTile && isThereStartButton) {
			// if there is already a start tile I can't add another one
			return;
		}
		else if (selectingMode == SelectingMode::EndTile && isThereEndButton) {
			// if there is already an end tile I can't add another one
			return;
		}

		//if in this button there is a start position remove start position
		//Note that if I am adding a start position there is not a start button already, otherwise I had returned before
		if (statusGrid[y][x] == SelectingMode::StartTile) {
			isThereStartButton = false;
		}
		//if in this button there is an end position remove end position
		//Note that if I am adding an end position there is not an end button already, otherwise I had returned before
		if (statusGrid[y][x] == SelectingMode::EndTile) {
			isThereEndButton = false;
		}

		statusGrid[y][x] = selectingMode;

		switch (selectingMode)
		{
		case ClearTile:
			button->texture() = whiteTexture;
			break;
		case StartTile:
			button->texture() = startTexture;
			isThereStartButton = true;
			break;
		case EndTile:
			button->texture() = endTexture;
			isThereEndButton = true;
			break;
		case BlockTile:
			button->texture() = blockTexture;
			break;
		default:
			std::cout << "Should never enter here" << std::endl;
			break;
		}
	}
}

void onClearClick(Button* button) {
	if (!executing && selectingMode != SelectingMode::ClearTile) {
		selectingMode = SelectingMode::ClearTile;

		btnClear->texture() = whiteClickedTexture;
		btnStart->texture() = startTexture;
		btnEnd->texture() = endTexture;
		btnBlock->texture() = blockTexture;
	}
}

void onStartClick(Button* button) {
	if (!executing && selectingMode != SelectingMode::StartTile) {
		selectingMode = SelectingMode::StartTile;

		btnClear->texture() = whiteTexture;
		btnStart->texture() = startClickedTexture;
		btnEnd->texture() = endTexture;
		btnBlock->texture() = blockTexture;
	}
}

void onEndClick(Button* button) {
	if (!executing && selectingMode != SelectingMode::EndTile) {
		selectingMode = SelectingMode::EndTile;

		btnClear->texture() = whiteTexture;
		btnStart->texture() = startTexture;
		btnEnd->texture() = endClickedTexture;
		btnBlock->texture() = blockTexture;
	}
}

void onBlockClick(Button* button) {
	if (!executing && selectingMode != SelectingMode::BlockTile) {
		selectingMode = SelectingMode::BlockTile;

		btnClear->texture() = whiteTexture;
		btnStart->texture() = startTexture;
		btnEnd->texture() = endTexture;
		btnBlock->texture() = blockClickedTexture;
	}
}

void onBeginClick(Button* button) {
	if (!executing && isThereStartButton && isThereEndButton) {
		std::cout << "Begin Clicked" << std::endl;
		elapsedTimeSinceLastUpdate = 0.0;
		executing = true;
		button->texture() = beginClickedTexture;

		initializeGraph();
		if (dijkstra) {
			shortestPathStrategy.reset(new Dijkstra(graph, gridXButtons * gridYButtons));
		}
		else{

			shortestPathStrategy.reset(new AStar(graph, gridXButtons * gridYButtons));
		}
	}
}

void getButtonXYFromIndex(int index, int& x, int& y) {
	x = index % gridXButtons;
	y = index / gridXButtons;
}

int getIndexFromXY(int x, int y) {
	return x + y * gridXButtons;
}

void initializeGraph() {
	graph.reset(new Graph(gridXButtons * gridYButtons));

	for (int y = 0; y != statusGrid.size(); ++y) {
		const auto& row = statusGrid[y];
		for (int x = 0; x != row.size(); ++x) {
			//int index = getIndexFromXY(x, y);
			int index = graph->nodes().size(); // index equals the position where the node is going to be added
			Graph::Node node = { index, x, y, -1 };
			//add node to graph
			graph->nodes().push_back(node);
			graph->adjacencyList().push_back(std::list<int>());

			//if this node is a block don't add adjacency
			if (row[x] == SelectingMode::BlockTile) {
				continue;
			}

			//add adjacency nodes
			int topAdjacent = getTopAdjacentButtonIndex(x, y);
			if (topAdjacent >= 0) {
				graph->adjacencyList()[index].push_back(topAdjacent);
			}

			int bottomAdjacent = getBottomAdjacentButtonIndex(x, y);
			if (bottomAdjacent >= 0) {
				graph->adjacencyList()[index].push_back(bottomAdjacent);
			}

			int leftAdjacent = getLeftAdjacentButtonIndex(x, y);
			if (leftAdjacent >= 0) {
				graph->adjacencyList()[index].push_back(leftAdjacent);
			}

			int rightAdjacent = getRightAdjacentButtonIndex(x, y);
			if (rightAdjacent >= 0) {
				graph->adjacencyList()[index].push_back(rightAdjacent);
			}

			//check if this node is start or end one
			if (row[x] == SelectingMode::StartTile) {
				graph->startNode() = index;
			}
			else if (row[x] == SelectingMode::EndTile) {
				graph->endNode() = index;
			}
		}
	}
}

int getTopAdjacentButtonIndex(int x, int y) {
	if (y <= 0 || statusGrid[y - 1][x] == SelectingMode::BlockTile) {
		return -1;
	}
	else {
		return getIndexFromXY(x, y - 1);
	}
}

int getBottomAdjacentButtonIndex(int x, int y) {
	if ((y+1) >=gridYButtons || statusGrid[y + 1][x] == SelectingMode::BlockTile) {
		return -1;
	}
	else {
		return getIndexFromXY(x, y + 1);
	}
}

int getLeftAdjacentButtonIndex(int x, int y) {
	if (x <= 0 || statusGrid[y][x-1] == SelectingMode::BlockTile) {
		return -1;
	}
	else {
		return getIndexFromXY(x-1, y);
	}
}

int getRightAdjacentButtonIndex(int x, int y) {
	if ((x+1) >= gridXButtons || statusGrid[y][x + 1] == SelectingMode::BlockTile) {
		return -1;
	}
	else {
		return getIndexFromXY(x + 1, y);
	}
}

void drawPath(const std::vector<int>& parents) {
	int parent = parents[graph->endNode()];
	while (parent >= 0) {
		int x = 0;
		int y = 0;
		getButtonXYFromIndex(parent, x, y);
		if (parent == graph->startNode()) {
			grid->buttons()[y][x].texture() = startTexture;
			break;
		}

		grid->buttons()[y][x].texture() = endClickedTexture;

		parent = parents[parent];
	}
}
