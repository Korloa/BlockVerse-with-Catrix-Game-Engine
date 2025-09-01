/*
 * Copyright (c) 2025 Kaixiang Zhang (’≈ø≠œË). All rights reserved.
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
	console.info("Exiting the netGameStarter State.");   
	return true;
}

bool NetGameStarter::enter(GameController* game) {
	console.info("Entering the netGameStater State.");
	return true;
}