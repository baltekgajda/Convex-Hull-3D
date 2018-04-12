#pragma once
#include "ConvexHull.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Facet;

class Edge
{
	Vertex *a, *b, *connected;
	Edge *next, *twin;
	Facet *facet;

public:
	Edge(Vertex &source, Vertex &destination, Facet &face);
	Edge(Vertex &source, Vertex &destination, Vertex & con);
	Edge(const Edge &edge);
	~Edge();

	bool ifMatches(Vertex& source, Vertex& destination);

	std::string toString();
	Vertex* getA();
	Vertex* getB();
	Vertex& getConnected();
	Edge* getNext();
	void setNext(Edge *e);
	Edge* getTwin();
	void setTwin(Edge *e);
	Facet* getFacet();
};
