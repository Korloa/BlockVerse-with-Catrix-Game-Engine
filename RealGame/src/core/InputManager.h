/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
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

//������Ӧ
struct keyEvent {
	int key;
	int action;
	int mods;
};							

//����ƶ�
struct mouseMoveEvent {
	double mouseX, mouseY;
};

//��갴��
struct mouseButtonEvent {
	int button;
	int action;
	int mods;
	double mouseX, mouseY;
};

//������
struct mouseScrollEvent {
	double xoffset, yoffset;
};

//using���ͱ���
using keyCallback = std::function<void(const keyEvent&)>;
using mouseMoveCallback = std::function<void(const mouseMoveEvent&)>;
using mouseButtonCallback = std::function<void(const mouseButtonEvent&)>;
using mouseScrollCallback = std::function<void(const mouseScrollEvent&)>;

using callbackId = uint64_t;

class InputManager {
private:
	friend class GameController;
	//GameController���Է���InputManager��˽�г�Ա����ֹ�ƻ�InputManager�ķ�װ��
	InputManager() = default;
	//����������Ĭ�Ϻ���,����ȫ

	//����״̬
	std::unordered_map<int, bool> keyStates;
	std::unordered_map<int, bool> previousKeyStates;  
	std::unordered_map<int, bool> mouseButtonStates;
	std::unordered_map<int, bool> previousMouseButtonStates;
	//�ص��б�
	std::unordered_map<callbackId,keyCallback> keyCallbacks;
	std::unordered_map<callbackId,mouseButtonCallback> mouseButtonCallbacks;
	std::unordered_map<callbackId,mouseScrollCallback> mouseScrollCallbacks;
	std::unordered_map<callbackId,mouseMoveCallback> mouseMoveCallbacks;
	

	std::unordered_set<callbackId> delSet;

	//�������
	double mouseX = 0.0f;
	double mouseY = 0.0f;

	callbackId nextId = 0;

	//�ַ��¼�,����Ҫglfw�ܹ����ܵ���ƥ��
	static void sendKeyCallback(GLFWwindow* window,int key,int scanCode,int action,int mods);
	static void sendMouseButtonCallback(GLFWwindow* window,int key,int action ,int mods);
	static void sendMouseMoveCallback(GLFWwindow* window, double mouseX,double mouseY);
	static void sendMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
	//��������
	static InputManager& getInstance() {
		static InputManager instance;
		return instance;
	}

	bool wasDelete(callbackId id) { return delSet.find(id) != delSet.end(); }

	callbackId allotId() { return nextId++; }

	InputManager(const InputManager&) = delete;
	//����������������ֹһ�����������InputManagerȥ����
	InputManager& operator=(const InputManager&) = delete;

	//��ʼ�����󶨴���
	void initialize(GLFWwindow* window);

	//��ѯAPI
	bool isKeyPressed(int key)const;
	bool isKeyDown();

	//ע�������
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



