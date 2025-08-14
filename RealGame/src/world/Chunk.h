/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#pragma once
#ifndef _CHUNK_H_
#define _CHUNK_H_
#include "utils/Noise.h"
#include "debug/Console.h"
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

const int chunkSize = 32;
const int chunkHeight = 256;
const int horzion = 128;

enum blockType {
	BASE = -1,
	AIR = 0,
	GRASS = 1,
	DIRT = 2,
	WATRE = 3,
	STONE = 4,
	IRON = 5,
	GOLD = 6,
	DIAMAND = 7,
	CLOUD = 8
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoords;
};

class Chunk {
public:
	int x, z;
	std::vector<std::vector<std::vector<int>>> blocks;
	unsigned int VAO, VBO, EBO;
	unsigned int vertexCount;
	bool needsUpdate;

	Chunk(int chunkX, int chunkZ, int worldSeed);
	~Chunk();

	void generateTerrain();
	void updateMesh();
	void render();
	int getBlock(int x, int y, int z) const;
	int setBlock(int x, int y, int z, blockType type);
	bool isBlockExposed(int x, int y, int z) const;

private:
	int seed;
	Noise noise;
	//初始化OpenGL缓存区域
	void setupMesh();
	void addFace(std::vector<Vertex>& vertices,
		std::vector<unsigned int>& indices,
		const glm::vec3& normal,
		const std::vector <glm::vec3>& position,
		const std::vector<glm::vec2>& texCoords);
};
#endif