/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include "states/basic/State.h"

class SlideState : public State {
private:
	float timer;		//��ʱ��
	std::vector<std::string>imagePath;	//ͼƬ·��
	float alpha;		//͸��
	int imageIndex;	//��ǰͼƬ����

	float lastTime;		//������ʾʱ��
	float transition;	//��������ʱ��

	GLFWwindow* mainWindow;
public:
	SlideState();
	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;

	bool enter(GameController* game) override;
	bool exit(GameController* game) override;
};