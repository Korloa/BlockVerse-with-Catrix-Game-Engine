/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [World.cpp]
 * Description: [Help connect all the chunks and make them work,optimize the system by dynamically loading and unlioading the area.]
 */
#include "world/World.h"


#include "states/loadingState/LoadingState.h"
#include "core/GameController.h"

World::World(int seed):seed(seed) {
	console.info("Initializing the world...");
	totalCount = (2 * renderDistance + 1) * (2 * renderDistance + 1);
}

World::~World() {
	for (auto& pair : chunks)
		delete pair.second;
}
void World::startGenerate() {
	std::thread([this]() {generateWorldAsync(); }).detach();
}

void World::generateWorldAsync() {
	generatedCount = 0;
	for (int x = -renderDistance; x <= renderDistance; x++) {
		for (int z = -renderDistance; z <= renderDistance; z++) {
			std::pair<int, int> key = { x,z };
			if (chunks.find(key) == chunks.end()) {

				Chunk* chunk = new Chunk(x, z, seed);

				generateChunk(chunk);

				{
					std::lock_guard<std::mutex> lock(genMutex);
					chunks[key] = chunk;
				}

				generatedCount++;
				LoadingState::getInstance().setProgress((float)generatedCount / (float)totalCount);
			}
		}
	}
	generationComplete = true;

	LoadingState::getInstance().exit(gameInstance);
}

void World::generateChunk(Chunk* chunk) {
	chunk->generateTerrain();
	chunk->buildMesh();
}

void World::render(Shader& shader) {
	renderCount = 0;
	for (auto& pair : chunks) {
		int chunkX = pair.first.first;
		int chunkZ = pair.first.second;
		if (pair.second->meshBuilt && pair.second->VAO == 0) {
			pair.second->uploadMesh();
		}

		if (pair.second->VAO != 0) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkX * chunkSize, 0, chunkZ * chunkSize));
			shader.setMat4("model", model);
			pair.second->render();
		}
		renderCount++;
	}
}
// 注意这里的x,z是块的索引，而不是世界坐标
Chunk* World::getChunk(int x,int z) {
	std::pair<int, int>key = { x,z };
	auto it = chunks.find(key);
	if (it != chunks.end())
		return it->second;
	//没有找到
	return nullptr;
}

void World::setBlock(int x,int y,int z,blockType type) {
	int chunkX = x / chunkSize;
	int chunkZ = z / chunkSize;
	int localX = x % chunkSize;
	int localZ = z % chunkSize;

	if (localX < 0)
		localX = localX - 1 + chunkSize;
	if (localZ < 0)
		localZ = localZ - 1 + chunkSize;

	Chunk* chunk = getChunk(chunkX, chunkZ);
	if (chunk)
		chunk->setBlock(localX, y, localZ, type);
}
////chunkX,chunkZ的编码
//long long World::getChunkKey(int x, int z)const {
//	return ((long long)x << 32) | (unsigned int)z;
//} 弃用