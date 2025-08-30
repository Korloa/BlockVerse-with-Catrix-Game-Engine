/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */


#pragma once
#include<string>

enum class windowMode {		//ǿ����ö�٣���������ʽת��
	FULLSCREEN,
	WINDOWED,
	BORDERLESS
};

struct gameConfig {

	std::string author;
	std::string coreVersion;
	std::string commandVersion;


	int windowWidth;
	int windowHeight;

	int frameBufferWidth;
	int frameBufferHeight;

	bool vSync;

	windowMode winMode;

	std::string theme;

	std::string language;

	float masterVolume;		//������	��BGM����Ч��������Ҫ�������õ����յ�����
	float musicVolume;		//BGM
	float sfxVolume;			//��Ч����


	float contentScale;


	//default settings
	gameConfig() {
		//Info
		author = "Kaixiang Zhang";
		coreVersion = "V1.2.0wa";
		commandVersion = "V2.0.0";


		//Setting
		windowWidth = 1920;
		windowHeight = 1080;

		frameBufferWidth = 1920;
		frameBufferHeight = 1080;

		winMode = windowMode::FULLSCREEN;

		theme = "default";

		language = "en";

		masterVolume = 1.0f;
		musicVolume = 1.0f;
		sfxVolume = 1.0f;

		contentScale = 1.0f;

		vSync = true;
	}
};