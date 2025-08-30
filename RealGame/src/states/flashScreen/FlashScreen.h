/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include "states/basic/State.h"


class FlashScreen : public State {
public:
	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;
	bool enter(GameController* game) override;
	bool exit(GameController* game) override;//���������û��ƥ����麯���ͻᱨ������ƴд����

	GLFWwindow* flashWindow;

	float startTime;
private:
	float alpha;
	float ease(float t) { return (t * t * (3.0f - 2.0f * t)); }
};