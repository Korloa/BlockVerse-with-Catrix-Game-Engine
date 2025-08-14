/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Noise.cpp]
 * Description: [Value noise generator with output in [-1, 1]]
 */

#include "utils/Noise.h"
#include "debug/Console.h"
#include <cmath>

Noise::Noise(int seed) : seed(seed) {}

float Noise::hash(int x, int z) const {
    // ʹ��������� + ����λ��ϣ����� h*h ��������ƫ
    long long h = (long long)x * 3747613LL +
        (long long)z * 6682651LL +
        (long long)seed * 9876543LL;

    // λ��ϣ�����Wang hash��Murmur���
    h ^= h >> 13;
    h *= 12741261LL;
    h ^= h >> 17;
    h *= 1013904223LL;  // ������
    h ^= h >> 13;

    // ȡ��31λ��Ϊ������������������⣩
    int value = (int)(h & 0x7FFFFFFF);  // �õ� [0, 2^31-1]

    // ӳ�䵽 [0.0, 1.0)����תΪ [-1.0, 1.0)
    return 2.0f * (value / 2147483647.0f) - 1.0f;
}

float Noise::getValue(float x, float z) const {
    int x0 = (int)floor(x);
    int z0 = (int)floor(z);
    int x1 = x0 + 1;
    int z1 = z0 + 1;

    float sx = x - x0;
    float sz = z - z0;

    // ƽ����ֵ��S-curve�����β�ֵ��
    sx = sx * sx * (3.0f - 2.0f * sx);
    sz = sz * sz * (3.0f - 2.0f * sz);

    // �ĸ��ǵ�����ֵ�������� [-1,1] ��Χ��
    float n00 = hash(x0, z0);
    float n10 = hash(x1, z0);
    float n01 = hash(x0, z1);
    float n11 = hash(x1, z1);

    // x �����ֵ
    float nx0 = (1.0f - sx) * n00 + sx * n10;
    float nx1 = (1.0f - sx) * n01 + sx * n11;

    // z �����ֵ
    float result = (1.0f - sz) * nx0 + sz * nx1;

    // ȷ������� [-1, 1] ��
    return result;
}

float Noise::getFractal(float x, float z, int octaves, float persistence, float scale) const {
    float total = 0.0f;
    float freq = scale;
    float amplitude = 1.0f;
    float maxValue = 0.0f;  // ���ڹ�һ����ȷ������� [-1,1]

    for (int i = 0; i < octaves; ++i) {
        total += getValue(x * freq, z * freq) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        freq *= 2.0f;
    }

    // ��һ���� [-1, 1]
    return total / maxValue;
}