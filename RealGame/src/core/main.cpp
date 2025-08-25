/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [main.cpp]
 * Description: [Application entry point]
 */

//Code::STAT API KorloaCode SFMyNTY.UzI5eWJHOWgjI01qVTVOVFk9.JJDTpit5wYSNClmT8SeeZK4wUPKfYWPebnfVXwmTq_Q

//ĿǰΪ���߳�ģʽ���Ժ����������Ҫ�ö��߳����Ż�����ƿ�������̵߳ļ��������Ż�����



#include <iostream>
#include "debug/Console.h"
#include "core/GameController.h"


#include "test/Test.h"


//���ݲ���: --width --height --fullscreen
int main(int argc,char* argv[]) {
	console.info("Main Application has started.");

	//TEST_MODE

	GameController mainGame;

	if (!mainGame.initialize()) {
		console.error("An Error happened while initializing the mainGame.");
		return -1;
	}

	mainGame.run();

	//��дSLEEP
	console.info("Main Application exited normally.");

	return 0;
}

