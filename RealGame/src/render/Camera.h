/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "debug/Console.h"

class Camera {
public:
	//相机的三维坐标
	glm::vec3 cameraPosition;
	// 相机的向前，向上，向右的方向向量
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	//世界向上方向
	glm::vec3 worldUp;
	//相机的水平，垂直旋转角
	float cameraYaw;
	float cameraPitch;
	//移动速度
	float mouseMoveSpeed;
	//视角灵敏度
	float mouseSensitivity;
	// FOV Field of Review
	float cameraZoom;

	glm::mat4 getViewMatrix();
	//处理键盘输入，direction枚举 deltaTime保证移动速度与帧率无关
	void procKey(int direction, float deltaTime);
	//处理鼠标移动
	void procMouseMove(float xoffset, float yoffset, bool constrainPitch = true);
	//处理鼠标缩放
	void procMouseScroll(float yoffset);
	Camera(	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			      glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),
			         float yaw = -90.0f,
				   float pitch = 0.0f);

	void updateCameraVectors();

};