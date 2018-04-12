#pragma once
#include "Facet.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <list>
#include <algorithm>

class Edge;
class Facet;
class Vertex;

class ConvexHull
{
	std::vector<Vertex> vertices;
	std::list<Facet*> facets;		
	Facet* firstTetrahedron[4];
	std::vector<Facet*> indexArray;

	std::vector<Vertex> voxelize(std::vector<glm::vec3> vert, double width);
	void findFirstTetrahedron();
	void createConflictList(int index, Facet * f1, Facet * f2, Facet *f3);
	void addConflicts(Facet &face, Vertex &ver);
	void deleteConflicts(Vertex &vertex);
	Vertex getLowestVertex();
	Vertex findSecondVertex(Vertex &vertex);
	int getNextVertex(Edge &edge);

public:
	ConvexHull(std::vector<glm::vec3> vert, double prec);
	~ConvexHull();

	std::vector<GLuint> incremental();
	std::vector<GLuint> giftWrapping();
	std::vector<GLuint> bruteForce();
};
