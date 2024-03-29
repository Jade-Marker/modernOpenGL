#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "GL/glew.h"
#include "glm/mat4x4.hpp"

class Shader
{
private:
	GLuint _shaderID;

	GLuint CompileShader(GLuint type, const std::string& sourcePath);
	GLuint CreateShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void BindShader();
	int GetUniformLocation(const std::string& uniformName);
	void SetUniformMatrix(const glm::mat4& matrix, std::string uniformName);
	void SetUniformInt(int value, std::string uniformName);
	void SetUniformVec3(const glm::vec3& vector, std::string uniformName);
	void SetUniformVec4(const glm::vec4& vector, std::string uniformName);
	void SetUniformFloat(float value, std::string uniformName);
};

