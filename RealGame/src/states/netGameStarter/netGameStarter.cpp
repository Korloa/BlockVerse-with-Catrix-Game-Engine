/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "states/netGameStarter/NetGameStarter.h"

void NetGameStarter::update(GameController* game, float deltaTime) {

}

void NetGameStarter::render(GameController* game) {

}

bool NetGameStarter::exit(GameController* game) {
	console.info("Exiting the GameLoader State.");   
	return true;
}

bool NetGameStarter::enter(GameController* game) {
	console.info("Entering the GameLoader State.");
	return true;
}