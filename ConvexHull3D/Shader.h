#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>

class Shader
{
	GLuint programID;
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	void Use() const;
	GLuint getProgramID() const;
	std::string readShaderCode(const GLchar* shaderPath);
	GLuint compileShader(const GLchar* shaderCode, GLenum shaderType);
};



