/*
 * Copyright (c) 2025 Kaixiang Zhang (’≈ø≠œË). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

enum SINGLELOAD {
	DEFAULT,
	CUSTOM
};

#pragma once
#include "states/basic/State.h"
class SingleGameStarter : public State {
public:
	SingleGameStarter();
	~SingleGameStarter();

	bool enter(GameController* game) override;
	bool exit(GameController* game) override;

	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;
private:
	InputBox* seedInput;

	float buttonWidth = 400.0f;
	float buttonHeight = 45.0f;
};