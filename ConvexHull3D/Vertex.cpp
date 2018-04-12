#include "Vertex.h"
using namespace std;

Vertex::Vertex(glm::vec3 vec, long int pos)
{
	coordinates = vec;
	position = pos;
}

Vertex::Vertex(const Vertex &ver)
{
	coordinates = ver.coordinates;
	position = ver.position;
	index = ver.index;
	conflictFacets = ver.conflictFacets;
}

Vertex::~Vertex() {}

//calculates normal of the plane of three vertices in that order
glm::vec3 Vertex::calculateNormal(Vertex& first, Vertex& second, Vertex& third)
{
	return glm::normalize(glm::cross(second.getCoord() - first.getCoord(), third.getCoord() - first.getCoord()));
}

//adds new conflict
std::list<Facet*>::const_iterator Vertex::addConflict(Facet & facet)
{
	conflictFacets.push_back(&facet);
	return (--conflictFacets.end());
}

//check if conflict list of facets is empty
bool Vertex::isInside()
{
	if (conflictFacets.size() != 0)
		return false;
	return true;
}

//finds horizon of edges visible to that vertex
std::vector<Edge> Vertex::getHorizon()
{
	std::vector<Edge> horizon;
	Edge *pivot = nullptr, *first = nullptr;

	for (auto iter = conflictFacets.begin(), end = conflictFacets.end(); iter != end; iter++)
		(*iter)->setVisibility(true);

	for (auto iter = conflictFacets.begin(), end = conflictFacets.end(); iter != end; iter++)
	{									//looking for first edge on the horizon
		for (int l = 0; l < 3; l++)
		{
			if ((*iter)->getFacetEdge(l)->getTwin()->getFacet()->getVisibility() == false)
			{
				pivot = (*iter)->getFacetEdge(l);
				break;
			}
		}
		if (pivot != nullptr)
			break;
	}

	while (first != pivot)									//moving along horizon to find all edges
	{
		if (pivot->getTwin()->getFacet()->getVisibility())
			pivot = pivot->getTwin()->getNext();
		else
		{
			if (first == nullptr)
				first = pivot;
			horizon.push_back(*pivot);
			pivot = pivot->getNext();
		}
	}

	return horizon;
}

string Vertex::toString()
{
	string s = "[" + to_string(coordinates[0]) + ", " + to_string(coordinates[1]) + ", " + to_string(coordinates[2])
		+ "] Position: " + to_string(position);
	return s;
}

glm::vec3 Vertex::getCoord()
{
	return coordinates;
}

long int Vertex::getPosition()
{
	return position;
}

long int Vertex::getIndex()
{
	return index;
}

void Vertex::setIndex(long int x)
{
	index = x;
}

std::list<Facet*> Vertex::getConflictFacets()
{
	return conflictFacets;
}
