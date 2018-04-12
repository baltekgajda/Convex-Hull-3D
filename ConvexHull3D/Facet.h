#pragma once
#include "Vertex.h"
#include "Edge.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <list>

class Facet
{
	Vertex *facetVertices[3];
	Edge *facetEdges[3];
	glm::vec3 normalVector;
	bool visible;

	void createEdges();
	void orientCheck(Vertex control);
	Edge* getMatchingEdge(Vertex &a, Vertex &b);

public:
	std::list<Facet*>::const_iterator it;			
	std::list<std::pair<Vertex*, std::list<Facet*>::const_iterator>> conflictVertices;
	Facet(Vertex &a, Vertex &b, Vertex &c, Vertex &control);
	Facet(Vertex &a, Vertex &b, Vertex &c);
	Facet(const Facet &facet);
	~Facet();

	void connect(Facet &face, Vertex &a, Vertex &b);
	void connect(int no, Edge *e);
	void addConflict(Vertex &vertex, std::list<Facet*>::const_iterator iter);
	bool isVisible(Vertex control);
	bool getVisibility();
	void setVisibility(bool vis);
	Edge *getFacetEdge(int index);
	Vertex *getFacetVertex(int index);
	std::list<Facet*>::const_iterator getIterator();
	void setIterator(std::list<Facet*>::const_iterator iter);
};


