#include "ConvexHull.h"
using namespace std;

//constructor that voxelizes set of points and sets vertices vector
//parameters: vert - set of points
//			prec - precision of voxelization
ConvexHull::ConvexHull(std::vector<glm::vec3> vert, double prec)
{
	if (prec < 0.05f)
		prec = 0.05f;
	if (prec > 5.0f)
		prec = 5.0f;
	vertices = voxelize(vert, prec);
	for (size_t i = 0; i < vertices.size(); i++)						//index of the point in vertices array;
		vertices[i].setIndex(i);
	indexArray.resize(vertices.size());
}

ConvexHull::~ConvexHull()
{
	firstTetrahedron[0];
	firstTetrahedron[1];
	firstTetrahedron[2];
	firstTetrahedron[3];
}

//voxelize a set of  3D points 
std::vector<Vertex> ConvexHull::voxelize(std::vector<glm::vec3> vert, double width)
{
	//0.0135-10
	double voxCount;
	int x, y, z, voxelCount;
	std::vector<Vertex> voxeledVertices;

	if (modf(10 / width, &voxCount) == 0)
		voxelCount = (int)voxCount;
	else
		voxelCount = (int)voxCount + 1;

	//voxel_count max = 759
	glm::vec3 *** array = new glm::vec3**[voxelCount];
	for (int i = 0; i < voxelCount; i++)
	{
		array[i] = new glm::vec3 *[voxelCount];
		for (int j = 0; j < voxelCount; j++)
		{
			array[i][j] = new glm::vec3[voxelCount];
			for (int k = 0; k < voxelCount; k++)
				array[i][j][k] = glm::vec3(-10.0, -10.0, -10.0);		//create temp array initialized with -10
		}
	}

	for (size_t i = 0; i < vert.size(); i++)
	{

		x = (int)((vert[i][0] + 5) / width);
		if (x == voxelCount)											//if x is a max value - 10
			x--;
		y = (int)((vert[i][1] + 5) / width);
		if (y == voxelCount)
			y--;
		z = (int)((vert[i][2] + 5) / width);
		if (z == voxelCount)
			z--;
		if (array[x][y][z][0] == -10.0)									//add to vertices if its the first point
		{																//in this voxel
			array[x][y][z] = vert[i];
			Vertex v(vert[i], i);
			voxeledVertices.push_back(v);		
		}
	}

	for (int i = 0; i < voxelCount; i++)
	{
		for (int j = 0; j < voxelCount; j++)
			delete[] array[i][j];
		delete[] array[i];
	}
	delete[]array;

	//cout << "===================================================" << endl;
	//cout << "Voxel width: " << width << endl;
	//cout << "Voxel count: " << voxelCount*voxelCount*voxelCount << endl;
	//cout << "Vertices count after voxelization: " << voxeledVertices.size() << endl;
	//cout <<"==================================================="<<endl;

	return voxeledVertices;
}

//finds first tetrahedron - used for incremented algorithm, search for four points that create a tetrahedron
void ConvexHull::findFirstTetrahedron()
{
	//cross product=0 means that two vectors are parallel
	size_t i, j = 0;
	for (i = 2; i < vertices.size(); i++)				
	{
		glm::vec3 normal = Vertex::calculateNormal(vertices[0], vertices[1], vertices[i]);
		if (normal != glm::vec3(0, 0, 0))														//czeck if third is not colinear
		{																						
			for (j = i + 1; j < vertices.size(); j++)											//look for the fourth one 
				if (glm::dot(normal, vertices[j].getCoord() - vertices[0].getCoord()) != 0)		//cant be coplanar
				{
					Vertex temp = vertices[i];
					vertices[i] = vertices[2];
					vertices[2] = temp;
					temp = vertices[j];
					vertices[j] = vertices[3];
					vertices[3] = temp;
					break;
				}
			break;
		}
	}			

	if (i == vertices.size() || j == vertices.size())
		cout << "Not enough vertices to create convex hull" << endl;

	firstTetrahedron[0] = new Facet(vertices[0], vertices[1], vertices[2], vertices[3]);		//first four facets 
	firstTetrahedron[1] = new Facet(vertices[0], vertices[2], vertices[3], vertices[1]);
	firstTetrahedron[2] = new Facet(vertices[0], vertices[3], vertices[1], vertices[2]);
	firstTetrahedron[3] = new Facet(vertices[1], vertices[3], vertices[2], vertices[0]);

	for (int i = 0; i < 4; i++)
	{
		facets.push_back(firstTetrahedron[i]);
		firstTetrahedron[i]->setIterator(--facets.end());										//setting iterators to facets
	}

	auto it0 = facets.begin(), it1 = ++it0;
	auto it2 = ++it0, it3 = ++it0;
	it0 = facets.begin();
	for (size_t k = 4; k < vertices.size(); k++)												//initializing conflict graph
	{
		if ((*it0)->isVisible(vertices[k]))
			addConflicts((**it0), vertices[k]);
		if ((*it1)->isVisible(vertices[k]))
			addConflicts((**it1), vertices[k]);
		if ((*it2)->isVisible(vertices[k]))
			addConflicts((**it2), vertices[k]);
		if ((*it3)->isVisible(vertices[k]))
			addConflicts((**it3), vertices[k]);
	}

	(*it0)->connect((**it1), vertices[0], vertices[2]);											//connecting facets 
	(*it0)->connect((**it2), vertices[0], vertices[1]);											//of first tetrahedron
	(*it0)->connect((**it3), vertices[1], vertices[2]);
	(*it1)->connect((**it2), vertices[0], vertices[3]);
	(*it1)->connect((**it3), vertices[2], vertices[3]);
	(*it2)->connect((**it3), vertices[1], vertices[3]);
}

//updating coflict lists by checking visibility of facets f2 i f3
void ConvexHull::createConflictList(int index, Facet * f1, Facet * f2, Facet * f3)
{
	//for (size_t i = index + 1; i < vertices.size(); i++)
	//	if (f1->isVisible(vertices[i]))
	//		addConflicts(*f1, vertices[i]);
	//auto lol = f2->conflictVertices;
	//lol.insert((lol.end()), f3->conflictVertices.begin(), f3->conflictVertices.end());

	for (auto iter = (f2->conflictVertices).begin(), end = (f2->conflictVertices).end(); iter != end; iter++)
	{
		if ((f1->isVisible(*iter->first)) && (iter->first->getPosition()) != vertices[index].getPosition())
		{
			addConflicts(*f1, *iter->first);
			indexArray[((iter)->first)->getIndex()] = f1;
		}
	}

	for (auto iter = (f3->conflictVertices).begin(), end = (f3->conflictVertices).end(); iter != end; iter++)
	{
		if ((f1->isVisible(*iter->first)) && indexArray[((iter)->first)->getIndex()] != f1 && (iter->first->getPosition()) != vertices[index].getPosition())
		{
			addConflicts(*f1, *iter->first);
			indexArray[((iter)->first)->getIndex()] = f1;
		}
	}
}

//adds conflicts to face and vertex
void ConvexHull::addConflicts(Facet & face, Vertex & ver)
{
	std::list<Facet*>::const_iterator iter = ver.addConflict(face);
	face.addConflict(ver, iter);
}

//delete all conflicts after deleting old facets
void ConvexHull::deleteConflicts(Vertex & vertex)
{
	while ((&vertex.conflictFacets)->size() != 0)
	{
		auto iter = (&vertex.conflictFacets)->begin();
		int i = 0;
		for (auto iter1 = (&((*iter)->conflictVertices))->begin(), end1 = (&((*iter)->conflictVertices))->end(); iter1 != end1; iter1++)
		{
			if (i == 0)					//deleting from facets vector
			{
				facets.erase((*iter)->it);
				i++;
			}
			(&((&(*iter1))->first->conflictFacets))->erase((&(*iter1))->second);
		}
	}
}

//used for gift wrapping algorithm, finds lowest point on the point cloud
Vertex ConvexHull::getLowestVertex()
{
	Vertex *lowest = &vertices[0];
	for (size_t i = 1; i < vertices.size(); i++)
	{
		if (vertices[i].getCoord()[1] < lowest->getCoord()[1])
			lowest = &vertices[i];
		else if (vertices[i].getCoord()[1] == lowest->getCoord()[1])
		{
			if (vertices[i].getCoord()[2] < lowest->getCoord()[2])
				lowest = &vertices[i];
			else if (vertices[i].getCoord()[2] == lowest->getCoord()[2])
			{
				if (vertices[i].getCoord()[0] < lowest->getCoord()[0])
					lowest = &vertices[i];
			}
		}
	}
	return *lowest;
}

//finds second lowest point in regard to angle to lowest point
Vertex ConvexHull::findSecondVertex(Vertex &vertex)
{
	glm::vec3 vec1, vec2;
	float angle = 0, temp = 0;
	int index = -1;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].getCoord() == vertex.getCoord())
			continue;

		vec1 = vertices[i].getCoord() - vertex.getCoord();
		vec2 = vec1;
		vec1[1] = 0.0f;

		temp = glm::dot(glm::normalize(vec1), glm::normalize(vec2));
		if (temp > angle)
		{
			angle = temp;
			index = i;
		}
	}
	return vertices[index];
}

//looking for next vertex with biggest angle from the edge
int ConvexHull::getNextVertex(Edge &edge)
{
	glm::vec3 a = edge.getA()->getCoord(), b = edge.getB()->getCoord();
	glm::vec3 vector = glm::normalize(b - a);
	int index = -1;										//index of the candidate 
	glm::vec3 candidate, temp;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].getCoord() == a || vertices[i].getCoord() == b)
			continue;

		if (vertices[i].getPosition() == edge.getConnected().getPosition())	//if point is already on convex hull
			continue;

		if (index == -1)
		{
			index = i;
			candidate = vertices[i].getCoord() - a;
			candidate -= glm::dot(candidate, vector)*vector;
		}
		else
		{
			temp = vertices[i].getCoord() - a;								
			temp -= glm::dot(temp, vector)*vector;

			if (glm::dot(glm::cross(candidate, temp), vector) > 0)				//finding next candidate
			{
				index = i;
				candidate = temp;
			}
		}
	}
	return index;
}

//---------------------------------------------------------------------------------------------------------------

//creates convex hull using incremental algorithm
std::vector<GLuint> ConvexHull::incremental()
{

	findFirstTetrahedron();
	Edge *rightEdge = nullptr, *firstEdge = nullptr;
	std::vector<Edge> horizon;										//holds horizon of edges visible to particular vertex
	std::vector<GLuint> indices;
	for (size_t i = 4; i < vertices.size(); ++i)
	{
		if (vertices[i].isInside())									//if vertex is inside it is not considered to be on convex hull
			continue;

		rightEdge = nullptr;
		firstEdge = nullptr;

		horizon = vertices[i].getHorizon();
		for (size_t j = 0; j < horizon.size(); j++)
		{
			Facet *face = new Facet(*(horizon[j].getA()), *(horizon[j].getB()), vertices[i]);	//creating facets from horizon
			face->connect(0, horizon[j].getTwin());
			facets.push_back(face);
			face->setIterator(--facets.end());
			createConflictList(i, face, horizon[j].getTwin()->getFacet(), horizon[j].getFacet());	//create new conflicts
			if (firstEdge == nullptr)										//creating connection between facets and edges
			{
				firstEdge = face->getFacetEdge(2);
				rightEdge = face->getFacetEdge(1);
			}
			else
			{
				rightEdge->setTwin(face->getFacetEdge(2));
				face->getFacetEdge(2)->setTwin(rightEdge);
				rightEdge = face->getFacetEdge(1);
			}
		}

		rightEdge->setTwin(firstEdge);
		firstEdge->setTwin(rightEdge);
		deleteConflicts(vertices[i]);								//deleting connections of deleted facets
	}

	for (auto it = facets.begin(), end = facets.end(); it != end; it++)
	{
		indices.push_back((*it)->getFacetVertex(0)->getPosition());
		indices.push_back((*it)->getFacetVertex(1)->getPosition());
		indices.push_back((*it)->getFacetVertex(2)->getPosition());
	}
	return indices;
}

//creates convex hull using gift wrapping algorithm
std::vector<GLuint> ConvexHull::giftWrapping()
{
	vector<Edge> edges;
	Vertex first = getLowestVertex();
	Vertex second = findSecondVertex(first);
	std::vector<GLuint> indices;
	int index = getNextVertex(Edge(first, second, second));				//looking for third point, second as conected vertex
																		//because there are no triangles yet
	indices.push_back(first.getPosition());
	indices.push_back(second.getPosition());
	indices.push_back(vertices[index].getPosition());

	edges.push_back(Edge(second, first, vertices[index]));
	edges.push_back(Edge(first, vertices[index], second));
	edges.push_back(Edge(vertices[index], second, first));
	while (edges.size() != 0)											//edges holds all edges that have only one side occupied by 
	{																	//a triangle, but at the and all edges should have to triangles 
		index = getNextVertex(edges.back());							//connected to them

		indices.push_back(edges.back().getA()->getPosition());
		indices.push_back(edges.back().getB()->getPosition());
		indices.push_back(vertices[index].getPosition());

		bool d1 = false, d2 = false;
		//checking if its already in edge vector if so the edge will be removed from vector
		for (size_t i = 0; i < edges.size() - 1; i++)
		{
			if (edges[i].getA()->getCoord() == vertices[index].getCoord() && edges[i].getB()->getCoord() == edges.back().getA()->getCoord())
			{
				edges.erase(edges.begin() + i);
				d1 = true;
				break;
			}
		}

		for (size_t i = 0; i < edges.size() - 1; i++)
		{
			if (edges[i].getB()->getCoord() == vertices[index].getCoord() && edges[i].getA()->getCoord() == edges.back().getB()->getCoord())
			{
				edges.erase(edges.begin() + i);
				d2 = true;
				break;
			}
		}

		int last = edges.size() - 1;

		if (!d1)
			edges.push_back(Edge(*edges[last].getA(), vertices[index], *edges[last].getB()));

		if (!d2)
			edges.push_back(Edge(vertices[index], *edges[last].getB(), *edges[last].getA()));

		edges.erase(edges.begin() + last);			//erasing last element
	}
	return indices;
}

//---------------------------------------------------------------------------------------------------------------

//creates convex hull using brute force algorithm
std::vector<GLuint> ConvexHull::bruteForce()
{
	glm::vec3 normal;
	int visible = 0, inside = 0, invisible = 0;
	float control;
	std::vector<GLuint> indices;

	for (size_t i = 0; i < vertices.size() - 2; i++)					//n-2
		for (size_t j = i + 1; j < vertices.size() - 1; j++)
			for (size_t k = j + 1; k < vertices.size(); k++)
			{
				normal = glm::cross(vertices[j].getCoord() - vertices[i].getCoord(), vertices[k].getCoord() - vertices[i].getCoord());
				for (size_t l = 0; l < vertices.size(); l++)
				{
					if (l == i || l == j || l == k)
						continue;

					control = glm::dot(vertices[l].getCoord() - vertices[k].getCoord(), normal);
					if (control > 0)						
						visible++;										//if facets is on convex hull, all points should be 
					else if (control == 0)								//either inside or outside a facet
						inside++;
					else
						invisible++;
				}

				if ((visible + inside) == vertices.size() - 3 || (invisible + inside) == vertices.size() - 3)
				{	
					indices.push_back(vertices[i].getPosition());							//add created facets
					indices.push_back(vertices[j].getPosition());
					indices.push_back(vertices[k].getPosition());
				}

				visible = 0;
				invisible = 0;
				inside = 0;
			}
	return indices;
}

