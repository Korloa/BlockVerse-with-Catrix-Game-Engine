/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

//10th-commit
//将CPU与GPU任务分离，并将CPU子线程化
#include "world/Chunk.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "states/loadingState/LoadingState.h"

Chunk::Chunk(int chunkX, int chunkZ,int worldSeed) :x(chunkX), z(chunkZ), needsUpdate(true),noise(seed),seed(worldSeed){
	blocks.resize(chunkSize, std::vector<std::vector<int>>(chunkHeight, std::vector<int>(chunkSize, AIR)));
}

//VAO Vertex Array Object	顶点数组对象
//VBO Vetex Buffer Object	顶点缓冲对象
//EBO Element Buffer Object 元素缓冲对象
//第一个参数统一指定生成个数
//第二个参数统一指定保存ID的变量，取地址
Chunk::~Chunk() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


void Chunk::generateTerrain() {
	std::ostringstream oss;
	oss << "Generating Terrain for chunk("<< this->x << "," << this->z << ")...";
	console.info(oss.str());
	LoadingState::getInstance().setText(oss.str());
	int orginx = this->x * chunkSize;
	int orginy = this->z * chunkSize;
	for (int ix = 0; ix < chunkSize; ix++) {
		for (int iz = 0; iz < chunkSize; iz++) {
		/*	int worldX = (this->x * chunkSize) + this->x < 0 ? chunkSize - ix - 1 : ix;
			int worldZ = (this->z * chunkSize) + this->z < 0 ? chunkSize - iz - 1 : iz;*/
			int worldX = (this->x * chunkSize) +ix;
			int worldZ = (this->z * chunkSize) +iz;
			int height = horzion + 32 * noise.getFractal(worldX, worldZ, 6, 0.5f, 0.007f);

			for (int iy = 0; iy < chunkHeight; iy++) {
				if (iy <= 2) {
					blocks[ix][iy][iz] = BASE;	//基岩层
				}
				else if (iy > 2 && iy < height - 3) {
					blocks[ix][iy][iz] = STONE;//石头层
				}
				else if (iy >= height - 3 && iy < height) {
					blocks[ix][iy][iz] = DIRT;	//泥土层
				}
				else if (iy == height) {
					blocks[ix][iy][iz] = GRASS;//草地
				}
				else if (iy > height && iy<chunkHeight-2) {
					blocks[ix][iy][iz] = AIR;	//空气
				}
				else {
					blocks[ix][iy][iz] = AIR;//云朵
				}
			}
		}
	}
	needsUpdate = true;
}

void Chunk::buildMesh() {
	//if (!needsUpdate || meshBuilt)
	//	return;

	/*std::ostringstream oss;
	oss << "Generating mesh for chunk(" << this->x << "," << this->z << ")...";
	console.info(oss.str());
	LoadingState::getInstance().setText(oss.str());*/

	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkHeight; y++) {
			for (int z = 0; z < chunkSize; z++) {
				if (blocks[x][y][z] != AIR && isBlockExposed(x, y, z)) {
					// 初始化左下角坐标
					glm::vec3 pos(x, y, z);
					//这里 isBlockExposed判断和里面的z== y== x== chunksize-1 0 有冲突
					// 这里不能用 if else-if
					// 注意判断的顺序问题 防止越界
					// +Z
					// ???括号不匹配，本地定义函数是非法的???
					if (z == chunkSize - 1 || blocks[x][y][z + 1] == AIR) {
						std::vector<glm::vec3> frontPositon = {
							pos + glm::vec3(0,0,1),
							pos + glm::vec3(1,0,1),
							pos + glm::vec3(1,1,1),
							pos + glm::vec3(0,1,1)
						};
						std::vector<glm::vec2> frontTexCoords = {
							glm::vec2(0,0),
							glm::vec2(0,1),
							glm::vec2(1,1),
							glm::vec2(1,0)
						};
						addFace(vertices, indices, glm::vec3(0, 0, 1), frontPositon, frontTexCoords);
					}
					// -Z
					if (z == 0 || blocks[x][y][z - 1] == AIR) {
						std::vector<glm::vec3> backPositon = {
							pos + glm::vec3(0,0,0),
							pos + glm::vec3(0,1,0),
							pos + glm::vec3(1,1,0),
							pos + glm::vec3(1,0,0)
						};
						std::vector<glm::vec2> backTexCoords = {
							glm::vec2(0,0),
							glm::vec2(1,0),
							glm::vec2(1,1),
							glm::vec2(0,1)
						};
						addFace(vertices, indices, glm::vec3(0, 0,-1), backPositon, backTexCoords);
					}
					// +Y
					if (y == chunkSize - 1 || blocks[x][y+1][z] == AIR) {
						std::vector<glm::vec3> topPositon = {
							pos + glm::vec3(0,1,0),
							pos + glm::vec3(1,1,0),
							pos + glm::vec3(1,1,1),
							pos + glm::vec3(0,1,1)
						};
						std::vector<glm::vec2> topTexCoords = {
							glm::vec2(0,0),
							glm::vec2(1,0),
							glm::vec2(1,1),
							glm::vec2(0,1)
						};
						addFace(vertices, indices, glm::vec3(0, 1, 0), topPositon, topTexCoords);
					}
					// -Y
					if (y == 0 || blocks[x][y-1][z] == AIR) {
						std::vector<glm::vec3> bottomPositon = {
							pos + glm::vec3(0,0,0),
							pos + glm::vec3(1,0,0),
							pos + glm::vec3(1,0,1),
							pos + glm::vec3(0,0,1)
						};
						std::vector<glm::vec2> bottomTexCoords = {
							glm::vec2(0,0),
							glm::vec2(0,1),
							glm::vec2(1,1),
							glm::vec2(1,1)
						};
						addFace(vertices, indices, glm::vec3(0, -1, 0), bottomPositon, bottomTexCoords);
					}
					// +X
					if (x == chunkSize - 1 || blocks[x+1][y][z] == AIR) {
						std::vector<glm::vec3> rightPositon = {
							pos + glm::vec3(1,0,0),
							pos + glm::vec3(1,0,1),
							pos + glm::vec3(1,1,1),
							pos + glm::vec3(1,1,0)
						};
						std::vector<glm::vec2> rightTexCoords = {
							glm::vec2(0,0),
							glm::vec2(0,1),
							glm::vec2(1,1),
							glm::vec2(1,0)
						};
						addFace(vertices, indices, glm::vec3(1, 0, 0),rightPositon,rightTexCoords);
					}
					// -X
					if (x == 0 || blocks[x-1][y][z] == AIR) {
						std::vector<glm::vec3> leftPositon = {
							pos + glm::vec3(0,0,0),
							pos + glm::vec3(0,0,1),
							pos + glm::vec3(0,1,1),
							pos + glm::vec3(0,1,0)
						};
						std::vector<glm::vec2> leftTexCoords = {
							glm::vec2(0,0),
							glm::vec2(0,1),
							glm::vec2(1,1),
							glm::vec2(1,0)
						};
						addFace(vertices, indices, glm::vec3(-1, 0, 0), leftPositon, leftTexCoords);
					}
				}
				// IF END

			}
		}
	}
	//FOR END

	meshBuilt = true;
	needsUpdate = false;


	vertexCount = indices.size();
}


void Chunk::uploadMesh() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// 将顶点放入缓存
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// 将索引放入缓存
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	//VAO只需要配置VBO，不需要配置EBO,因为EBO的读取方式比较单一，unsigned int,？默认约定好连续的三个为一个面
	//而VAO不一样，它有位置，纹理坐标，法向量等要素，比较复杂，需要专门配置

	//激活三个索引来保存 position,textCoords,normal
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));

	//配置读取方式
	// what fuck is (void*)0?
	//索引值-索引分量数-分量类型-是否归一化-步长(单个分量大小)-偏移量
	//offsetof与结构体中变量的定义顺序密切相关

	// 显示松绑，避免状态泄露
	glBindVertexArray(0);
}

// addFace 添加一个面
// complex 5 veribles 
// vertices		存储所有顶点的数据
// indices		存储索引数据
// normal		表示面的法向
// position		4个顶点的位置坐标
// texCoords	4个顶点的纹理坐标
void Chunk::addFace(std::vector<Vertex>& vertices,
		std::vector<unsigned int>& indices,
		const glm::vec3& normal,
		const std::vector <glm::vec3>& position,
		const std::vector<glm::vec2>& texCoords){

	unsigned int startIndex = vertices.size();
	// 记录向量大小,vertices存储整个Chunk的顶点
	// startIndex记录当前面的4个顶点在列表中的起始位置
	// E.g. StartIndex =100 100 101 102 103
	for (int i = 0; i < 4; i++) {
		Vertex vertex;
		vertex.position = position[i];
		// 4个顶点共用一个法线
		vertex.normal = normal;
		vertex.textCoords = texCoords[i];
		vertices.push_back(vertex);
	}

	// 一个正方形由两个三角形组成
	// 第一个三角形
	indices.push_back(startIndex);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 2);
	// 第二个三角形
	indices.push_back(startIndex);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 3);
	//indices告诉OpenGL如何将顶点连接成三角形
}

void Chunk::render() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

int Chunk::getBlock(int x,int y,int z)const {
	if (x >= 0 && x < chunkSize && z >= 0 && z <= chunkSize && y >= 0 && y <= chunkHeight)
		return blocks[x][y][z];
	return OUT_OF_ARRAY_SIZE_ERROR;
}

int Chunk::setBlock(int x, int y, int z, blockType type) {
	if (x >= 0 && x < chunkSize && z >= 0 && z <= chunkSize && y >= 0 && y <= chunkHeight) {
		blocks[x][y][z] = type;
		needsUpdate = true;
	}
	return OUT_OF_ARRAY_SIZE_ERROR;
}
//注意此函数判断存在缺陷，对于区块边缘的方块是无法处理的
bool Chunk::isBlockExposed(int x, int y, int z) const{
	// x方向 
	if (x > 0 && blocks[x - 1][y][z] == AIR)
		return true;
	else if (x < chunkSize - 1 && blocks[x + 1][y][z] == AIR)
		return true;
	// y方向
	else if (y > 0 && blocks[x][y - 1][z] == AIR)
		return true;
	else if (y < chunkHeight - 3 && blocks[x][y + 1][z] == AIR)	//后续补全云的情况，但感觉暂时不需要，因为128+-64，最大192，不可能256-3
		return true;
	// z方向
	else if (z > 0 && blocks[x][y][z - 1] == AIR)
		return true;
	else if (z < chunkSize - 1 && blocks[x][y][z + 1] == AIR)
		return true;

	// 没有暴露
	return false;
}

