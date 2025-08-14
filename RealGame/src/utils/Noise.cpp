/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Noise.cpp]
 * Description: [Value noise generator with output in [-1, 1]]
 */

#include "utils/Noise.h"
#include "debug/Console.h"
#include <cmath>

Noise::Noise(int seed) : seed(seed) {}

float Noise::hash(int x, int z) const {
    // 使用线性组合 + 优质位混合，避免 h*h 带来的正偏
    long long h = (long long)x * 3747613LL +
        (long long)z * 6682651LL +
        (long long)seed * 9876543LL;

    // 位混合（类似Wang hash或Murmur风格）
    h ^= h >> 13;
    h *= 12741261LL;
    h ^= h >> 17;
    h *= 1013904223LL;  // 大质数
    h ^= h >> 13;

    // 取低31位作为正整数（避免符号问题）
    int value = (int)(h & 0x7FFFFFFF);  // 得到 [0, 2^31-1]

    // 映射到 [0.0, 1.0)，再转为 [-1.0, 1.0)
    return 2.0f * (value / 2147483647.0f) - 1.0f;
}

float Noise::getValue(float x, float z) const {
    int x0 = (int)floor(x);
    int z0 = (int)floor(z);
    int x1 = x0 + 1;
    int z1 = z0 + 1;

    float sx = x - x0;
    float sz = z - z0;

    // 平滑插值：S-curve（三次插值）
    sx = sx * sx * (3.0f - 2.0f * sx);
    sz = sz * sz * (3.0f - 2.0f * sz);

    // 四个角的噪声值（现在在 [-1,1] 范围）
    float n00 = hash(x0, z0);
    float n10 = hash(x1, z0);
    float n01 = hash(x0, z1);
    float n11 = hash(x1, z1);

    // x 方向插值
    float nx0 = (1.0f - sx) * n00 + sx * n10;
    float nx1 = (1.0f - sx) * n01 + sx * n11;

    // z 方向插值
    float result = (1.0f - sz) * nx0 + sz * nx1;

    // 确保结果在 [-1, 1] 内
    return result;
}

float Noise::getFractal(float x, float z, int octaves, float persistence, float scale) const {
    float total = 0.0f;
    float freq = scale;
    float amplitude = 1.0f;
    float maxValue = 0.0f;  // 用于归一化，确保输出在 [-1,1]

    for (int i = 0; i < octaves; ++i) {
        total += getValue(x * freq, z * freq) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        freq *= 2.0f;
    }

    // 归一化到 [-1, 1]
    return total / maxValue;
}