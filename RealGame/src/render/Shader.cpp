/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#include "render/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	console.info("Initializing the shader system...");

	// 读取顶点着色器
	std::string vertexCode;
	std::ifstream vertexShaderFile;
	const char* c_vertexCode;
	// ostream sstream iostream ifstream oss ???
	// void() const
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vertexShaderFile.open(vertexPath);
		std::stringstream vertexShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		vertexShaderFile.close();
		vertexCode = vertexShaderStream.str();
		c_vertexCode = vertexCode.c_str();
		console.info("Successfully access to vertex shader file.");
	}
	catch (std::ifstream::failure& e) {
		console.error("Vertex shader file can not successfully access.");
	}

	// 读取片段着色器

	std::string fragmentCode;
	std::ifstream fragmentShaderFile;
	const char* c_fragmentCode;
	
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		fragmentShaderFile.open(fragmentPath);
		std::stringstream fragmentShaderStream;
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		fragmentShaderFile.close();
		fragmentCode = fragmentShaderStream.str();
		c_fragmentCode = fragmentCode.c_str();
		console.info("Successfully access to vertex fragment shader file.");
	}
	catch (std::ifstream::failure& e) {
		console.error("Fragment shader file can not successfully access.");
	}

	unsigned int vertex, fragment;	//ID

	console.info("Compiling vertex shader...");
	// 编译顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &c_vertexCode, NULL);
	glCompileShader(vertex);
	checkCompileError(vertex, "VERTEX");


	console.info("Compiling fragment shader...");
	//	编译片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &c_fragmentCode, NULL);
	glCompileShader(fragment);
	checkCompileError(fragment, "FRAGMENT");


	// 合并
	console.info("Linking shader files...");
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileError(ID, "PROGRAM");

	// 消除中间件

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader::use() {
	// glcreateProgram -> glUseProgram 
	glUseProgram(ID);
}

//？
void Shader::setMat4(const std::string& name,const glm::mat4& mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setInt(const std::string& name, int value)const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::checkCompileError(unsigned int shader, std::string type) {
	// program是链接阶段
	int success;
	char log[1024];
	if (type == "PROGRAM") {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, log);
			std::ostringstream oss;
			oss << "An error happened while linking program shader." << std::endl << "Shader type:[" + type + "]" << std::endl<<"Detailed log:"<<log;
			console.error(oss.str());
		}
	}
	// shader是编译阶段
	else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, log);
			std::ostringstream oss;
			oss << "An error happened while compiling program shader." << std::endl << "Shader type:[" + type + "]" << std::endl << "Detailed log:" << log;
		}
	}
}

