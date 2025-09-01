/*
 * Copyright (c) 2025 Kaixiang Zhang (’≈ø≠œË). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */
#pragma once
#include "render/2dObj/RectRender.h"
#include "render/text/TextRender.h"
#include "core/InputManager.h"

#include <string>
#include <functional>

class InputBox {
public:
	InputBox();
	~InputBox();
	
	bool initialize(const std::string& fontPath, unsigned int fontSize,glm::vec2 pos);
	void render();
	void update(float deltaTime);

	void onMouseClick(int x, int y);
	void onKeyPress(const keyEvent& event);
	//void onCharInput(unsigned int codepoint);

	void drawCursor();

	std::string getValue() { return text; }
private:
	glm::vec2 position;
	glm::vec2 size;

	std::string text;

	int cursorPos;

	std::map<int, char> keyToCharMap;
	std::map<int, char> keyToCharShiftMap;

	bool isFocused;

	bool cursorVisible;

	float cursorBlinkTime;

	callbackId keyId;

	callbackId clickId;

	int maxLength = 16;
};