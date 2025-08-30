/*
 * Copyright (c) 2025 Kaixiang Zhang (’≈ø≠œË). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include "states/basic/State.h"

class NetGameStarter : public State {
public:
	bool enter(GameController* game) override;
	bool exit(GameController* game) override;

	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;


};