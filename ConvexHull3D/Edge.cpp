#include "Edge.h"
using namespace std;

Edge::Edge(Vertex &source, Vertex &destination, Facet &face)
	:a(&source), b(&destination), facet(&face)
{
	next = nullptr;
	twin = nullptr;
	connected = nullptr;
}

Edge::Edge(Vertex & source, Vertex & destination, Vertex & con)
	:a(&source), b(&destination), connected(&con)
{
	facet = nullptr;
	next = nullptr;
	twin = nullptr;
}

Edge::Edge(const Edge &edge)
	:a(edge.a), b(edge.b), connected(edge.connected)
{
	facet = edge.facet;
	next = edge.next;
	twin = edge.twin;
}

Edge::~Edge() {}

//checking if this edges source and destination matches arguments
bool Edge::ifMatches(Vertex& source, Vertex& destination)
{
	return ((a == &source && b == &destination) || (a == &destination && b == &source));
}

string Edge::toString()
{
	string s = "Edge: " + a->toString() + " " + b->toString();
	return s;
}

Vertex * Edge::getA()
{
	return this->a;
}

Vertex * Edge::getB()
{
	return this->b;
}

Vertex & Edge::getConnected()
{
	return *connected;
}

Edge * Edge::getNext()
{
	return next;
}

void Edge::setNext(Edge *e)
{
	next = e;
}

Edge * Edge::getTwin()
{
	return twin;
}

void Edge::setTwin(Edge *e)
{
	twin = e;
}

Facet * Edge::getFacet()
{
	return facet;
}






