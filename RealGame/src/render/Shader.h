/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "debug/Console.h"

class Shader {
public:
	unsigned int ID;
	// OpenGL会为着色器程序分配唯一的ID
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setMat4(const std::string& name,const glm::mat4& mat) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setInt(const std::string& name, int value) const;
private:
	void checkCompileError(unsigned int shader, std::string type);

};

#endif
