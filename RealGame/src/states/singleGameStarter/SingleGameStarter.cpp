/*
 * Copyright (c) 2025 Kaixiang Zhang (’≈ø≠œË). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "states/singleGameStarter/SingleGameStarter.h"

void SingleGameStarter::update(GameController* game, float deltaTime) {

}

void SingleGameStarter::render(GameController* game) {

}

bool SingleGameStarter::exit(GameController* game) {
	console.info("Exiting the GameLoader State.");
	return true;
}

bool SingleGameStarter::enter(GameController* game) {
	console.info("Entering the GameLoader State.");
	return true;
}