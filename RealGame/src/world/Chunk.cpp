/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

//10th-commit
//��CPU��GPU������룬����CPU���̻߳�
#include "world/Chunk.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "states/loadingState/LoadingState.h"

Chunk::Chunk(int chunkX, int chunkZ,int worldSeed) :x(chunkX), z(chunkZ), needsUpdate(true),noise(seed),seed(worldSeed){
	blocks.resize(chunkSize, std::vector<std::vector<int>>(chunkHeight, std::vector<int>(chunkSize, AIR)));
}

//VAO Vertex Array Object	�����������
//VBO Vetex Buffer Object	���㻺�����
//EBO Element Buffer Object Ԫ�ػ������
//��һ������ͳһָ�����ɸ���
//�ڶ�������ͳһָ������ID�ı�����ȡ��ַ
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
					blocks[ix][iy][iz] = BASE;	//���Ҳ�
				}
				else if (iy > 2 && iy < height - 3) {
					blocks[ix][iy][iz] = STONE;//ʯͷ��
				}
				else if (iy >= height - 3 && iy < height) {
					blocks[ix][iy][iz] = DIRT;	//������
				}
				else if (iy == height) {
					blocks[ix][iy][iz] = GRASS;//�ݵ�
				}
				else if (iy > height && iy<chunkHeight-2) {
					blocks[ix][iy][iz] = AIR;	//����
				}
				else {
					blocks[ix][iy][iz] = AIR;//�ƶ�
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
					// ��ʼ�����½�����
					glm::vec3 pos(x, y, z);
					//���� isBlockExposed�жϺ������z== y== x== chunksize-1 0 �г�ͻ
					// ���ﲻ���� if else-if
					// ע���жϵ�˳������ ��ֹԽ��
					// +Z
					// ???���Ų�ƥ�䣬���ض��庯���ǷǷ���???
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

	// ��������뻺��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// ���������뻺��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	//VAOֻ��Ҫ����VBO������Ҫ����EBO,��ΪEBO�Ķ�ȡ��ʽ�Ƚϵ�һ��unsigned int,��Ĭ��Լ��������������Ϊһ����
	//��VAO��һ��������λ�ã��������꣬��������Ҫ�أ��Ƚϸ��ӣ���Ҫר������

	//������������������ position,textCoords,normal
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));

	//���ö�ȡ��ʽ
	// what fuck is (void*)0?
	//����ֵ-����������-��������-�Ƿ��һ��-����(����������С)-ƫ����
	//offsetof��ṹ���б����Ķ���˳���������

	// ��ʾ�ɰ󣬱���״̬й¶
	glBindVertexArray(0);
}

// addFace ���һ����
// complex 5 veribles 
// vertices		�洢���ж��������
// indices		�洢��������
// normal		��ʾ��ķ���
// position		4�������λ������
// texCoords	4���������������
void Chunk::addFace(std::vector<Vertex>& vertices,
		std::vector<unsigned int>& indices,
		const glm::vec3& normal,
		const std::vector <glm::vec3>& position,
		const std::vector<glm::vec2>& texCoords){

	unsigned int startIndex = vertices.size();
	// ��¼������С,vertices�洢����Chunk�Ķ���
	// startIndex��¼��ǰ���4���������б��е���ʼλ��
	// E.g. StartIndex =100 100 101 102 103
	for (int i = 0; i < 4; i++) {
		Vertex vertex;
		vertex.position = position[i];
		// 4�����㹲��һ������
		vertex.normal = normal;
		vertex.textCoords = texCoords[i];
		vertices.push_back(vertex);
	}

	// һ�����������������������
	// ��һ��������
	indices.push_back(startIndex);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 2);
	// �ڶ���������
	indices.push_back(startIndex);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 3);
	//indices����OpenGL��ν��������ӳ�������
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
//ע��˺����жϴ���ȱ�ݣ����������Ե�ķ������޷������
bool Chunk::isBlockExposed(int x, int y, int z) const{
	// x���� 
	if (x > 0 && blocks[x - 1][y][z] == AIR)
		return true;
	else if (x < chunkSize - 1 && blocks[x + 1][y][z] == AIR)
		return true;
	// y����
	else if (y > 0 && blocks[x][y - 1][z] == AIR)
		return true;
	else if (y < chunkHeight - 3 && blocks[x][y + 1][z] == AIR)	//������ȫ�Ƶ���������о���ʱ����Ҫ����Ϊ128+-64�����192��������256-3
		return true;
	// z����
	else if (z > 0 && blocks[x][y][z - 1] == AIR)
		return true;
	else if (z < chunkSize - 1 && blocks[x][y][z + 1] == AIR)
		return true;

	// û�б�¶
	return false;
}

