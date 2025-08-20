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

//后期可以更改
const int renderDistance = 20;

class World {
public:
	int seed;

	std::map<std::pair<int,int>, Chunk*>chunks;

	World(int seed);
	~World();

	void generateWorld(int seed);

	void render(Shader& shader);
	Chunk* getChunk(int x, int z);

	void setBlock(int x, int y, int z,blockType type);
};

#endif