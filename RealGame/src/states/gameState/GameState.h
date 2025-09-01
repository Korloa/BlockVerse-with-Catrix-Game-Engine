/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include "states/basic/State.h"
#include "world/World.h"
#include "render/Camera.h"
#include "core/InputManager.h"

class GameState : public State {
public:
	GameState(int num);
	~GameState();

	float lastX;
	float lastY;
	bool firstMouse = true;

	bool enter(GameController* game) override;
	bool exit(GameController* game) override;

	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;

	std::function<void(const keyEvent&)>playerMove;

	//视角转动
	std::function<void(const mouseMoveEvent&)>headMove;


	//视角Zoom
	std::function<void(const mouseScrollEvent&)>headZoom;

private:
	float delta;
	int seed;
	Shader* worldShader;
	World* world;
	Camera* camera;
};