/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#ifndef _WORLD_H_
#define _WORLD_H_

#include "world/Chunk.h"
#include "render/Shader.h"

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <atomic>
#include <mutex>

//后期可以更改
const int renderDistance = 5;

class World {
public:
	int seed;

	int totalCount;

	std::map<std::pair<int,int>, Chunk*>chunks;

	World(int seed);
	~World();

	mutable std::mutex genMutex;
	std::atomic<bool> generationComplete{ false };

	std::atomic<int> generatedCount{ 0 };
	int renderCount;

	void startGenerate();
	void generateChunk(Chunk* chunk);
	void generateWorldAsync();


	void render(Shader& shader);
	Chunk* getChunk(int x, int z);

	void setBlock(int x, int y, int z,blockType type);
};

#endif