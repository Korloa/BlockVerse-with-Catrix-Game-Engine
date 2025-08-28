/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
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
	//�������ά����
	glm::vec3 cameraPosition;
	// �������ǰ�����ϣ����ҵķ�������
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	//�������Ϸ���
	glm::vec3 worldUp;
	//�����ˮƽ����ֱ��ת��
	float cameraYaw;
	float cameraPitch;
	//�ƶ��ٶ�
	float mouseMoveSpeed;
	//�ӽ�������
	float mouseSensitivity;
	// FOV Field of Review
	float cameraZoom;

	glm::mat4 getViewMatrix();
	//����������룬directionö�� deltaTime��֤�ƶ��ٶ���֡���޹�
	void procKey(int direction, float deltaTime);
	//��������ƶ�
	void procMouseMove(float xoffset, float yoffset, bool constrainPitch = true);
	//�����������
	void procMouseScroll(float yoffset);
	Camera(	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			      glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),
			         float yaw = -90.0f,
				   float pitch = 0.0f);

	void updateCameraVectors();

};