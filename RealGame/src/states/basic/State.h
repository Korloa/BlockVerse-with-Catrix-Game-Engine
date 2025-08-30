/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include<map>
#include<vector>

#include "components/Button.h"
#include "components/TextInput.h"

#include "core/GameController.h"

class TextRender;
class RectRender;

class State {
protected:
	std::vector<Button>sharedButtons;				//���а�ť��������ҳ���л����л�
	std::vector<int> pageStack;
	std::map<int, std::vector<Button>>pageButtons;
	//std::map<int, std::vector<TextInput>>pageTextInputs;	//��δʵ��
	std::map<int, float>keyTimes; //ȥ����ʱ

public:
	virtual ~State() = default;
	virtual void procInput(GameController* game, float deltaTime) {}
	virtual void update(GameController* game, float deltaTime) = 0;
	virtual void render(GameController* game) = 0;//����0�������Ҫʵ��
	virtual bool enter(GameController* game) { return true; }
	virtual bool exit(GameController* game) { return true; }		//{}���಻һ��Ҫʵ��

	virtual void removeInputCallbacks(GLFWwindow* gameWindow) {}
	virtual void setInputCallbacks(GLFWwindow* gameWindow) {}

	bool isKeyPressed(GameController* game, int key, float debounceTime = 0.2f) {
		float currentTime = (float)glfwGetTime();
		if (glfwGetKey(game->getWindowInstance(), key) == GLFW_PRESS &&
			(!keyTimes.count(key) || currentTime - keyTimes[key] > debounceTime)) {
			//��������õ��Ƿ�ֹ�̰���ʶ��Ϊ��Σ�������Ч���ǳ�ʱ�䰴����������ԵĻظ�����Ϊ 32��һֱ��ˢ��ʱ��
			keyTimes[key] = currentTime;
			return true;
		}
		return false;
	}
};    