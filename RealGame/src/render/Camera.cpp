/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Camera.cpp]
 * Description: [The implementation of camera control.]
 */
#include "render/Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	cameraFront(glm::vec3(0.0f,0.0f,-1.0f)),mouseMoveSpeed(30.0f),mouseSensitivity(0.1f),cameraZoom(45.0f){

	console.info("Initializing the camera system...");

	cameraPosition = position;
	cameraUp = up;
	worldUp = up;
	cameraYaw = yaw;
	cameraPitch = pitch;
	updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));	
	front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	front.y = sin(glm::radians(cameraPitch));
	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));	//这里的顺序需要研究一下
}

void Camera::procMouseScroll(float yscroll) {
	cameraZoom += yscroll;
	if (cameraZoom < 1.0f)
		cameraZoom = 1.0f;
	else if (cameraZoom > 45.0f)
		cameraZoom = 45.0f;
}

void Camera::procMouseMove(float xoffset,float yoffset,bool constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	cameraYaw += xoffset;
	cameraPitch -= yoffset;
	if (constrainPitch) {
		if (cameraPitch > 89.0f)
			cameraPitch = 89.0f;
		else if (cameraPitch < -89.0f)
			cameraPitch = -89.0f;
	}
	updateCameraVectors();	//只有视角旋转才会变动Camera Vector
}

void Camera::procKey(int direction,float deltaTime) {
	float velocity = mouseMoveSpeed * deltaTime;	//velocity 真实速度 速率
	switch (direction) {
	case 0:
		cameraPosition += cameraFront * velocity;
		break;
	case 1:
		cameraPosition -= cameraFront * velocity;
		break;
	case 2:
		cameraPosition -= cameraRight * velocity;
		break;
	case 3:
		cameraPosition += cameraRight * velocity;
		break;
	case 4:
		cameraPosition += cameraUp * velocity;
		break;
	case 5:
		cameraPosition -= cameraUp * velocity;
		break;
	}

}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPosition,cameraPosition+cameraFront, cameraUp);
}