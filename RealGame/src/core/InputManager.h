/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "debug/Console.h"

//键盘响应
struct keyEvent {
	int key;
	int action;
	int mods;
};							

//鼠标移动
struct mouseMoveEvent {
	double mouseX, mouseY;
};

//鼠标按键
struct mouseButtonEvent {
	int button;
	int action;
	int mods;
	double mouseX, mouseY;
};

//鼠标滚动
struct mouseScrollEvent {
	double xoffset, yoffset;
};

//using类型别名
using keyCallback = std::function<void(const keyEvent&)>;
using mouseMoveCallback = std::function<void(const mouseMoveEvent&)>;
using mouseButtonCallback = std::function<void(const mouseButtonEvent&)>;
using mouseScrollCallback = std::function<void(const mouseScrollEvent&)>;

using callbackId = uint64_t;

class InputManager {
private:
	friend class GameController;
	//GameController可以访问InputManager的私有成员，防止破坏InputManager的封装性
	InputManager() = default;
	//编译器构造默认函数,更安全

	//按键状态
	std::unordered_map<int, bool> keyStates;
	std::unordered_map<int, bool> previousKeyStates;  
	std::unordered_map<int, bool> mouseButtonStates;
	std::unordered_map<int, bool> previousMouseButtonStates;
	//回调列表
	std::unordered_map<callbackId,keyCallback> keyCallbacks;
	std::unordered_map<callbackId,mouseButtonCallback> mouseButtonCallbacks;
	std::unordered_map<callbackId,mouseScrollCallback> mouseScrollCallbacks;
	std::unordered_map<callbackId,mouseMoveCallback> mouseMoveCallbacks;
	

	std::unordered_set<callbackId> delSet;

	//缓存鼠标
	double mouseX = 0.0f;
	double mouseY = 0.0f;

	callbackId nextId = 0;

	//分发事件,这里要glfw能够接受的相匹配
	static void sendKeyCallback(GLFWwindow* window,int key,int scanCode,int action,int mods);
	static void sendMouseButtonCallback(GLFWwindow* window,int key,int action ,int mods);
	static void sendMouseMoveCallback(GLFWwindow* window, double mouseX,double mouseY);
	static void sendMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
	//单例访问
	static InputManager& getInstance() {
		static InputManager instance;
		return instance;
	}

	bool wasDelete(callbackId id) { return delSet.find(id) != delSet.end(); }

	callbackId allotId() { return nextId++; }

	InputManager(const InputManager&) = delete;
	//拷贝函数声明，防止一个对象用这个InputManager去拷贝
	InputManager& operator=(const InputManager&) = delete;

	//初始化，绑定窗口
	void initialize(GLFWwindow* window);

	//轮询API
	bool isKeyPressed(int key)const;
	bool isKeyDown();

	//注册接收器
	callbackId addKeyCallback(const keyCallback& callback);
	callbackId addMouseButtonCallback(const mouseButtonCallback& callback);
	callbackId addMouseMoveCallback(const mouseMoveCallback& callback);
	callbackId addMouseScrollCallback(const mouseScrollCallback& callback);

	void removeKeyCallback(callbackId tarId);
	void removeMouseButtonCallback(callbackId tarId);
	void removeMouseScrollCallback(callbackId tarId);
	void removeMouseMoveCallback(callbackId tarId);

	void removeAllCallback();
};



