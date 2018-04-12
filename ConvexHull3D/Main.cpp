#include "View.h"
#include "ConvexHull.h"
#include "Generation.h"
#include <ctime>
#include <chrono>
#include <cmath>

using namespace std;
enum generatorCode { sp, sq, or };
enum methodCode { in, gw, bf };

int main()
{
	srand((unsigned int)time(NULL));
	int pointsCount = 10;
	int step = 1;
	int generations = 100;
	double precision = 0.0001;
	int generator = sp;					//TODO
	int method = in;

	vector<glm::vec3> vertices;
	vector<GLuint> indices;

	int teoComplexity;
	double teoMedianComplexity;

	double ** results = new double*[generations];
	for (int i = 0; i < generations; i++)
		results[i] = new double[3];

	for (int i = 0; i < generations; i++, pointsCount += step)
	{
		printProgress(i, generations);
		results[i][0] = pointsCount;

		switch (generator)
		{
		case sp:
			vertices = generateSphere(pointsCount);
			break;
		case sq:
			vertices = generateSquare(pointsCount);
			break;
		case or :
			vertices = generateOrbit(pointsCount, 24);
		}

		ConvexHull convexhull(vertices, precision);
		auto start = std::chrono::system_clock::now();
		switch (method)
		{
		case in:
			indices = convexhull.incremental();
			teoComplexity = (int)(pointsCount*log10(pointsCount));
			break;
		case gw:
			indices = convexhull.giftWrapping();
			teoComplexity = pointsCount*pointsCount;
			break;
		case bf:
			indices = convexhull.bruteForce();
			teoComplexity = pointsCount*pointsCount*pointsCount*pointsCount;
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> duration = end - start;
		results[i][1] = duration.count() * 1000;
		results[i][2] = results[i][1] / teoComplexity;
	}
	printProgress(generations, generations);
	int medianN;
	double medianTime;
	if (generations % 2 == 0)
	{
		medianN = (int)(results[generations / 2][0] + results[generations / 2 - 1][0]) / 2;
		medianTime = (results[generations / 2][1] + results[generations / 2 - 1][1]) / 2;
	}
	else
	{
		medianN = (int)results[generations / 2][0];
		medianTime = results[generations / 2][1];
	}

	switch (method)
	{
	case in:
		teoMedianComplexity = medianN*log10(medianN);
		break;
	case gw:
		teoMedianComplexity = medianN*medianN;
		break;
	case bf:
		teoMedianComplexity = medianN*medianN*medianN*medianN;
		break;
	}

	for (int i = 0; i < generations; i++)
		results[i][2] *= (teoMedianComplexity / medianTime);
	printResultsTable(results, generations);
	View view(vertices, indices);
	view.visualize();
	return 0;
}

