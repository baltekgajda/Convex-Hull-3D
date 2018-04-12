#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

std::vector<glm::vec3> generateSquare(long int count);
std::vector<glm::vec3> generateSphere(long int count);
std::vector<glm::vec3> generateOrbit(long int count, GLfloat smaller_radius);
void printProgress(int actual, int maximum);
void printResultsTable(double ** results, int size);
