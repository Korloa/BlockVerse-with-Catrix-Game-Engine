/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [main.cpp]
 * Description: [Application entry point]
 */

//Code::STAT API KorloaCode SFMyNTY.UzI5eWJHOWgjI01qVTVOVFk9.JJDTpit5wYSNClmT8SeeZK4wUPKfYWPebnfVXwmTq_Q

#include <iostream>
#include "debug/Console.h"
#include "core/Game.h"


#include "test/Test.h"


//传递参数: --width --height --fullscreen
int main(int argc,char* argv[]) {
	console.info("Main Application has started.");

	//TEST_MODE

	Game mainGame;

	if (!mainGame.initialize()) {
		console.error("An Error happened while initializing the mainGame.");
		return -1;
	}

	mainGame.run();

	//手写SLEEP
	console.info("Main Application exited normally.");

	return 0;
}

