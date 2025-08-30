/*
 * Copyright (c) 2025 Kaixiang Zhang (’≈ø≠œË). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "states/settingState/SettingState.h"

enum Setting {
	MAIN,
	SCREEN,
	GAME,
};


void SettingState::update(GameController* game, float deltaTime) {

}

void SettingState::render(GameController* game) {

}

bool SettingState::enter(GameController* game) {
	console.info("Entering the Settings State.");
	return true;
}

bool SettingState::exit(GameController* game) {
	console.info("Exiting the Settings State.");
	return true;
}