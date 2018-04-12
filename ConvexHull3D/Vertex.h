#pragma once
#include "Shader.h"
#include "Facet.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <list>

class Edge;
class Facet;

class Vertex
{
	glm::vec3 coordinates;
	long int position;							
	long int index;					

public:
	std::list<Facet*> conflictFacets;
	Vertex(glm::vec3 vec, long int pos);
	Vertex(const Vertex &ver);
	~Vertex();

	static glm::vec3 Vertex::calculateNormal(Vertex& first, Vertex& second, Vertex& third);
	std::list<Facet*>::const_iterator addConflict(Facet & facet);
	bool isInside();
	std::vector<Edge> getHorizon();

	std::string toString();
	glm::vec3 getCoord();
	long int getPosition();
	long int getIndex();
	void setIndex(long int x);
	std::list<Facet*> getConflictFacets();









};
