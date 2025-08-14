/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#pragma once
#include <cstddef>
// for size_t
class Noise {
public:
	// ����������ʼ������
	explicit Noise(int seed);
	// ���ɹ�һ����2Dֵ����
	float getValue(float x, float z) const;

	float getFractal(float x, float z,int octaves = 4, float persistence = 0.5f, float scale = 0.001f)const;
private:
	int seed;
	//��ϣ����
	float hash(int x, int z)const;
};