/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Game.cpp]
 * Description: [The detailed logic of the game,this may unit various states of the game.]
 */
#include "core/Game.h"
#include <iostream>

// 全局访问，用于回调函数
Game* gameInstance = nullptr;

Game::Game():gameWindow(nullptr),/*, shader(nullptr), camera(nullptr),*/
deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(400), lastY(300) {
	gameInstance = this;
}
Game::~Game() {
	cleanup();
}
bool Game::initialize() {

	/// TEMP CODE
	int tempSeed;
	console.info("Please input the seed");
	std::cin >> tempSeed;
	///


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gameWindow = glfwCreateWindow(1024,768,"MyCraft Game Window", NULL, NULL);
	if (gameWindow == NULL) {
		/*std::cout << "Fail to create GLFW window" << std::endl;*/
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gameWindow);

	glfwSetFramebufferSizeCallback(gameWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(gameWindow, mouse_callback);
	glfwSetScrollCallback(gameWindow, scroll_callback);

	glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK) {
		std::cout << "Fail to initialize GLEW" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	shader = new Shader("shader/vertex.glsl", "shader/fragment.glsl");

	camera = new Camera(glm::vec3(0.0f,(float)horzion,0.0f), glm::vec3(0.0f, 1.0f, 0.0f),0.0f,0.0f);

	world = new World(tempSeed);

	return true;
}

void::Game::run() {
	//主循环
	while (!glfwWindowShouldClose(gameWindow)) {

		console.info("Starting the gameLoop...");

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput();
		update();
		render();
		//???
		glfwSwapBuffers(gameWindow);
		//???
		glfwPollEvents();
	}
}

void::Game::cleanup() {

	console.info("Deleteing the sources...");

	delete world;
	delete camera;
	delete shader;
	glfwTerminate();
}

void::Game::processInput() {
	if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(gameWindow, true);
	if (glfwGetKey(gameWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera->procKey(0, deltaTime);
	if (glfwGetKey(gameWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera->procKey(1, deltaTime);
	if (glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera->procKey(2, deltaTime);
	if (glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera->procKey(3, deltaTime);
	if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->procKey(4, deltaTime);
	if (glfwGetKey(gameWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->procKey(5, deltaTime);
}
void::Game::update() {
	// update logic
}
void::Game::render() {
	glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();

	glm::mat4 projection = glm::perspective(glm::radians(camera->cameraZoom), 800.0f / 600.0f, 0.1f, 1000.0f);
	shader->setMat4("projection", projection);

	// 设置视图矩阵
	glm::mat4 view = camera->getViewMatrix();
	shader->setMat4("view", view);

	// 设置模型矩阵
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);

	shader->setVec3("lightPos", glm::vec3(0.0f, 256.0f, 0.0f));     // 光源位置
	shader->setVec3("viewPos", camera->cameraPosition);                     // 摄像机位置（用于高光，虽然现在没用）
	shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));       // 白光

	// 渲染世界
	world->render(*shader);
}
// framebuffer---viewport
void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (gameInstance->firstMouse) {
		gameInstance->firstMouse = false;
		gameInstance->lastX = xpos;
		gameInstance->lastY = ypos;
	}
	else {
		float xoffset = xpos - gameInstance->lastX;
		float yoffset = ypos - gameInstance->lastY;

		gameInstance->camera->procMouseMove(xoffset, yoffset);

		gameInstance->lastX = xpos;
		gameInstance->lastY = ypos;
	}
}
void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	gameInstance->camera->procMouseScroll(yoffset);
}