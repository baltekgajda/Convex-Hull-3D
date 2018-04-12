#include "Shader.h"
using namespace std;

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	// prepare shaders
	string vertexCode = readShaderCode(vertexPath);
	GLuint vertexID = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	string fragmentCode = readShaderCode(fragmentPath);
	GLuint fragmentID = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

	// link shader program first creates program id
	programID = glCreateProgram();
	//attach says which are connected
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	//link creates something like exe version, binding connected components
	glLinkProgram(programID);

	// Print linking errors if any
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
		string msg = string("Shader program linking: ") + infoLog;
		throw exception(msg.c_str());
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

Shader::~Shader() {}

void Shader::Use() const
{
	glUseProgram(getProgramID());
}

GLuint Shader::getProgramID() const
{
	return programID;
}

string Shader::readShaderCode(const GLchar * shaderPath)
{
	ifstream shaderFile;
	shaderFile.exceptions(ifstream::badbit);
	shaderFile.open(shaderPath);
	stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	return shaderStream.str();
}

GLuint Shader::compileShader(const GLchar * shaderCode, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	// Print compile errors if any
	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		string msg = string("Shader compilation: ") + infoLog;
		throw exception(msg.c_str());
	}
	return shaderID;
}

