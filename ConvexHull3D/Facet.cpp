#include "Facet.h"
using namespace std;

//creates edges of this particular facet, also sets order of edges
void Facet::createEdges()
{
	facetEdges[0] = new Edge(*facetVertices[0], *facetVertices[1], *this);
	facetEdges[1] = new Edge(*facetVertices[1], *facetVertices[2], *this);
	facetEdges[2] = new Edge(*facetVertices[2], *facetVertices[0], *this);
	facetEdges[0]->setNext(facetEdges[1]);
	facetEdges[1]->setNext(facetEdges[2]);
	facetEdges[2]->setNext(facetEdges[0]);
}

//control vertex should be invisible to facet, if it is not 
//normal vector is flipped as well as facet vertices
void Facet::orientCheck(Vertex control)
{
	if (isVisible(control))
	{
		Vertex *temp = facetVertices[1];
		facetVertices[1] = facetVertices[2];
		facetVertices[2] = temp;
		normalVector *= -1;
	}
}

//looking for matching edge in a facet with two vertices given
Edge* Facet::getMatchingEdge(Vertex & a, Vertex & b)
{
	for (int i = 0; i < 3; i++)
	{
		if (facetEdges[i]->ifMatches(a, b))
			return facetEdges[i];
	}
	return nullptr;
}

Facet::Facet(Vertex &a, Vertex &b, Vertex &c, Vertex &control)
{
	visible = false;
	facetVertices[0] = &a;
	facetVertices[1] = &b;
	facetVertices[2] = &c;
	normalVector = Vertex::calculateNormal(a, b, c);
	orientCheck(control);
	createEdges();
}

Facet::Facet(Vertex &a, Vertex &b, Vertex &c)
{
	visible = false;
	facetVertices[0] = &a;
	facetVertices[1] = &b;
	facetVertices[2] = &c;
	normalVector = Vertex::calculateNormal(a, b, c);
	createEdges();
}

Facet::Facet(const Facet & facet)
{
	visible = facet.visible;
	normalVector = facet.normalVector;
	for (int i = 0; i < 3; i++)
		facetVertices[i] = facet.facetVertices[i];
	conflictVertices = facet.conflictVertices;
	it = facet.it;

	createEdges();
	for (int i = 0; i < 3; i++)
	{
		facetEdges[i]->setTwin(facet.facetEdges[0]->getTwin());
		if (facetEdges[i]->getTwin() != nullptr)
			facetEdges[i]->getTwin()->setTwin(facetEdges[0]);
	}
}

Facet::~Facet()
{
	delete facetEdges[0];
	delete facetEdges[1];
	delete facetEdges[2];
}

//connects to another facet using two vertices
void Facet::connect(Facet & face, Vertex & a, Vertex & b)
{
	Edge *inner = getMatchingEdge(a, b);
	Edge *outer = face.getMatchingEdge(a, b);
	inner->setTwin(outer);
	outer->setTwin(inner);
}

//connect edge to particular edge on a facet
void Facet::connect(int no, Edge * e)
{
	facetEdges[no]->setTwin(e);
	e->setTwin(facetEdges[no]);
}

//adds conflict vertex to facet, iterator that lead to position of this facet
//in conflict list of the vertex
void Facet::addConflict(Vertex & vertex, std::list<Facet*>::const_iterator iter)
{
	conflictVertices.push_back(std::make_pair(&vertex, iter));
}

//check if control vertex is visible to facet
bool Facet::isVisible(Vertex control)
{
	if (glm::dot(control.getCoord() - facetVertices[0]->getCoord(), normalVector) >= 0)
		return true;
	return false;
}

bool Facet::getVisibility()
{
	return visible;
}

void Facet::setVisibility(bool vis)
{
	visible = vis;
}

Edge * Facet::getFacetEdge(int index)
{
	return facetEdges[index];
}

Vertex * Facet::getFacetVertex(int index)
{
	return facetVertices[index];
}

//iterator that shows place of position of this facet in facets list
std::list<Facet*>::const_iterator Facet::getIterator()
{
	return it;
}

void Facet::setIterator(std::list<Facet*>::const_iterator iter)
{
	it = iter;
}
