/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#pragma once
#include <cstddef>
// for size_t
class Noise {
public:
	// 用种子来初始化噪声
	explicit Noise(int seed);
	// 生成归一化的2D值噪声
	float getValue(float x, float z) const;

	float getFractal(float x, float z,int octaves = 4, float persistence = 0.5f, float scale = 0.001f)const;
private:
	int seed;
	//哈希函数
	float hash(int x, int z)const;
};