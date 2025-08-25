/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [main.cpp]
 * Description: [Application entry point]
 */

//Code::STAT API KorloaCode SFMyNTY.UzI5eWJHOWgjI01qVTVOVFk9.JJDTpit5wYSNClmT8SeeZK4wUPKfYWPebnfVXwmTq_Q

//目前为单线程模式，以后引擎成熟需要用多线程来优化性能瓶颈，多线程的几个可能优化方向



#include <iostream>
#include "debug/Console.h"
#include "core/GameController.h"


#include "test/Test.h"


//传递参数: --width --height --fullscreen
int main(int argc,char* argv[]) {
	console.info("Main Application has started.");

	//TEST_MODE

	GameController mainGame;

	if (!mainGame.initialize()) {
		console.error("An Error happened while initializing the mainGame.");
		return -1;
	}

	mainGame.run();

	//手写SLEEP
	console.info("Main Application exited normally.");

	return 0;
}

