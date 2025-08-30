/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once

#include "states/basic/State.h"
#include "render/Shader.h"
#include "render/text/TextRender.h"
#include "render/2dObj/RectRender.h"
#include "render/image/ImageRender.h"


class LoadingState :public State {
public:
	bool visible;

	void update(GameController* game, float deltaTime) override;
	void render(GameController* game)override;


	LoadingState();
	~LoadingState();
	//外部接口
	bool enter(GameController* game) override;
	bool exit(GameController* game)override;
	void setProgress(float num);
	void setText(const std::string& msg);

	static LoadingState& getInstance() {
		static LoadingState state;
		return state;
	}

	//LoadingState(const LoadingState&) = delete;
	//LoadingState& operator=(const LoadingState&) = delete;

private:
	const float progressWidth = 320.0f;
	const float progressHeight = 10.0f;
	const float slidingButtonWidth = 80.0f;

	float messageY = 540.0f+140.0f;
	float progressY = 540.0f+80.0f;

	float sliderOffset;

	float progress;
	std::string message;

	float rotateAngle;
	float rotateSpeed;



	std::vector<float>cubeVertices;
	std::vector<unsigned int>cubeIndices;

	unsigned int cubeVAO, cubeVBO, cubeEBO;

	Shader* cubeShader;
};