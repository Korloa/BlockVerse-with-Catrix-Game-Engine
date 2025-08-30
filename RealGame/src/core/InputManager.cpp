/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "core/InputManager.h"


//初始化
void InputManager::initialize(GLFWwindow* window) {
	glfwSetKeyCallback(window, InputManager::sendKeyCallback);
	glfwSetMouseButtonCallback(window, InputManager::sendMouseButtonCallback);
	glfwSetCursorPosCallback(window, InputManager::sendMouseMoveCallback);
	glfwSetScrollCallback(window, InputManager::sendMouseScrollCallback);
}
//注册事件
callbackId InputManager::addKeyCallback(const keyCallback& callback) {
	console.info("Key callback registered one.");
	callbackId newId = allotId();
	keyCallbacks[newId]=callback;
	return newId;
}

callbackId InputManager::addMouseButtonCallback(const mouseButtonCallback& callback) {
	console.info("Mouse button callback registered one.");
	callbackId newId = allotId();
	mouseButtonCallbacks[newId]=callback;
	return newId;
}
callbackId InputManager::addMouseScrollCallback(const mouseScrollCallback& callback) {
	console.info("Mouse scroll callback registered one.");
	callbackId newId = allotId();
	mouseScrollCallbacks[newId]=callback;
	return newId;
}
callbackId InputManager::addMouseMoveCallback(const mouseMoveCallback& callback) {
	console.info("Mouse move callback registered one.");
	callbackId newId = allotId();
	mouseMoveCallbacks[newId]=callback;
	return newId;
}


//转发函数
void InputManager::sendKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	InputManager& im = InputManager::getInstance();
	//为何加入外层,难道返回键值还能不稳定？
	if (key >= 0 && key <= GLFW_KEY_LAST) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			im.keyStates[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			im.keyStates[key] = false;
		}

		keyEvent event{key,action,mods};

		for (const auto& pair : im.keyCallbacks) {
			pair.second(event);
		}
	}
}

void InputManager::sendMouseButtonCallback(GLFWwindow* window, int key, int action, int mods) {
	InputManager& im = InputManager::getInstance();
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	mouseButtonEvent event{ key,action,mods,mouseX,mouseY };

	for (const auto& pair : im.mouseButtonCallbacks) {
		pair.second(event);
	}
}


void InputManager::sendMouseScrollCallback(GLFWwindow* windwo, double xoffset, double yoffset) {
	InputManager& im = InputManager::getInstance();
	
	mouseScrollEvent event{ xoffset,yoffset };

	for (const auto& pair : im.mouseScrollCallbacks) {
		pair.second(event);
	}
}

void InputManager::sendMouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY) {
	InputManager& im = InputManager::getInstance();
	
	mouseMoveEvent event{ mouseX,mouseY };

	for (const auto& pair : im.mouseMoveCallbacks) {
		pair.second(event);
	}
}

void InputManager::removeKeyCallback(callbackId removeId) {
	console.info("Key callback removed one.");
	keyCallbacks.erase(removeId);
}
void InputManager::removeMouseButtonCallback(callbackId removeId) {
	console.info("Mouse button callback removed one.");
	mouseButtonCallbacks.erase(removeId);
}
void InputManager::removeMouseMoveCallback(callbackId removeId) {
	console.info("Mouse move callback removed one.");
	mouseMoveCallbacks.erase(removeId);
}
void InputManager::removeMouseScrollCallback(callbackId removeId) {
	console.info("Mouse scroll callback removed one.");
	mouseScrollCallbacks.erase(removeId);
}


void InputManager::removeAllCallback() {
	mouseButtonCallbacks.clear();
	mouseMoveCallbacks.clear();
	mouseScrollCallbacks.clear();
	keyCallbacks.clear();
}

//void InputManager::update() {}; 这个函数和justPress绑定的，有点说法