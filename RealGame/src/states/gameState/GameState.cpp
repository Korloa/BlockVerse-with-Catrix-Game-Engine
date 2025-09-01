/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "states/gameState/GameState.h"
#include "states/loadingState/LoadingState.h"
#include <GLFW/glfw3.h>

GameState::GameState(int num):seed(num) {

}

GameState::~GameState() {

}

bool GameState::enter(GameController* game){
	glfwSetInputMode(game->getWindowInstance(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	InputManager& im = InputManager::getInstance();


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	playerMove = [&](const keyEvent& e) {
		if (e.action == GLFW_PRESS || e.action == GLFW_REPEAT) {
			if (e.key == GLFW_KEY_W)
				camera->procKey(0, delta);
			if (e.key == GLFW_KEY_S)
				camera->procKey(1, delta);
			if (e.key == GLFW_KEY_A)
				camera->procKey(2, delta);
			if (e.key == GLFW_KEY_D)
				camera->procKey(3, delta);
			if (e.key == GLFW_KEY_SPACE)
				camera->procKey(4, delta);
			if (e.key == GLFW_KEY_LEFT_SHIFT)
				camera->procKey(5, delta);
		}
		//GLFWwindow* gameWindow = game->getWindowInstance();

		//if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		//	glfwSetWindowShouldClose(gameWindow, true);
		//if (glfwGetKey(gameWindow, GLFW_KEY_W) == GLFW_PRESS)
		//	camera->procKey(0, delta);
		//if (glfwGetKey(gameWindow, GLFW_KEY_S) == GLFW_PRESS)
		//	camera->procKey(1, delta);
		//if (glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS)
		//	camera->procKey(2, delta);
		//if (glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS)
		//	camera->procKey(3, delta);
		//if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		//	camera->procKey(4, delta);
		//if (glfwGetKey(gameWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		//	camera->procKey(5, delta);
	};


	headMove = [&](const mouseMoveEvent& event) {
		if (firstMouse) {
			firstMouse = false;
			lastX = event.mouseX;
			lastY = event.mouseY;
		}
		else {
			float xoffset = event.mouseX - lastX;
			float yoffset = event.mouseY - lastY;

			camera->procMouseMove(xoffset, yoffset);

			lastX = event.mouseX;
			lastY = event.mouseY;
			camera->procMouseMove(xoffset * delta,yoffset * delta, true);
		}
	};

	headZoom = [&](const mouseScrollEvent& event) {
		camera->procMouseScroll(event.yoffset);
	};


	im.addKeyCallback(playerMove);
	im.addMouseMoveCallback(headMove);
	im.addMouseScrollCallback(headZoom);    

	worldShader = new Shader("render/vertex.glsl", "render/fragment.glsl");

	camera = new Camera(glm::vec3(0.0f, (float)horzion+30.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

	world = new World(seed);

	LoadingState::getInstance().enter(game);
	LoadingState::getInstance().setProgress(-1.0f);
	LoadingState::getInstance().setText("Loading...");

	world->startGenerate();

	return true;
}

void GameState::update(GameController* game, float deltaTime) {
	delta = deltaTime;
}

void GameState::render(GameController* game) {
	if (world->generationComplete.load()) {

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);	//深度测试的比较函数
		//GL_NEVER GL_LESS GL_LEQUAL GL_EQUAL GL_GREATER GL_GEQUAL GL_NOTEQUAL GL_ALWAYS
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // 天空蓝
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		worldShader->use();

		glm::mat4 projection = glm::perspective(
			glm::radians(camera->cameraZoom),
			(float)1920.0f / (float)1080.f,
			0.1f, 1000.0f
		);
		worldShader->setMat4("projection", projection);

		glm::mat4 view = camera->getViewMatrix();
		worldShader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		worldShader->setMat4("model", model);

		worldShader->setVec3("lightPos", glm::vec3(0.0f, 256.0f, 0.0f));
		worldShader->setVec3("viewPos", camera->cameraPosition);
		worldShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		world->render(*worldShader);

		glDisable(GL_DEPTH_TEST); // 避免 UI 被深度测试挡住
		glDisable(GL_CULL_FACE);

	}
}

bool GameState::exit(GameController* game) {
	console.info("Exiting the Gaming State.");
	return true;
}
