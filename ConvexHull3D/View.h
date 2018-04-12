#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <vector>

#define WIDTH 2000
#define HEIGHT 2000
#define POINT_SIZE 1.0f
#define LINE_WIDTH 1.0f

extern float camRotAngle;
extern int keysCounter;

enum class Key { Nothing, Left, Right, Up, Down };
class View
{
	std::vector<GLfloat> *vertices;
	std::vector<GLuint> &indices;
	Shader *pointsProgram;
	Shader *trianglesProgram;
	void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
	glm::mat4 camRotation(glm::vec4 camPosition, glm::vec3 camUpVector);
	void bindToVAO(GLuint VAO, GLuint VBO, GLuint EBO);
	void drawElements(GLuint VAO, glm::mat4 view, glm::mat4 projection);
	GLFWwindow* initialize();
public:
	View(std::vector<glm::vec3> &vert, std::vector<GLuint> &indi);
	void visualize();
	~View();
};

