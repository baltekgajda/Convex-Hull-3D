#include "Generation.h"
using namespace std;

//generate points cloud in as shape of square
vector<glm::vec3> generateSquare(long int points)
{
	if (points < 10)
		points = 10;

	vector<glm::vec3> square;
	std::vector<float> temp1, temp2, temp3;
	int count = 10;

	while (count + 1 < points)
		count *= 10;

	for (size_t i = 0; i < count; i++)
	{
		temp1.push_back(i);
		temp2.push_back(i);
		temp3.push_back(i);
	}

	std::random_shuffle(temp1.begin(), temp1.end());
	std::random_shuffle(temp2.begin(), temp2.end());
	std::random_shuffle(temp3.begin(), temp3.end());

	for (int i = 0; i < points; i++)
		square.push_back(glm::vec3(temp1[i] / (count / 10) - 5.0, temp2[i] / (count / 10) - 5.0, temp3[i] / (count / 10) - 5.0));

	return square;
}

//generate points cloud in as shape of sphere
vector<glm::vec3> generateSphere(long int points)
{
	if (points < 10)
		points = 10;

	vector<glm::vec3> sphere;
	std::vector<float> temp1, temp2, temp3;
	int count = 10;

	while (count + 1 < points)
		count *= 10;
	count *= 10;

	for (int i = 0; i < count; i++)
	{
		temp1.push_back(i);
		temp2.push_back(i);
		temp3.push_back(i);
	}

	std::random_shuffle(temp1.begin(), temp1.end());
	std::random_shuffle(temp2.begin(), temp2.end());
	std::random_shuffle(temp3.begin(), temp3.end());

	int i = 0;
	double x, y, z;
	while (sphere.size() < points && i < count)
	{

		x = (temp1[i] / (count / 10)) - 5.0;
		y = (temp2[i] / (count / 10)) - 5.0;
		z = (temp3[i] / (count / 10)) - 5.0;
		if (x*x + y*y + z*z <= 25.0)
			sphere.push_back(glm::vec3(x, y, z));
		i++;
	}

	return sphere;
}

//generate points cloud in as shape of orbit with smaller orbit given as an argument
vector<glm::vec3> generateOrbit(long int points, GLfloat smaller_radius)
{
	if (points < 10)
		points = 10;
	if (smaller_radius > 25.0f)
		smaller_radius = 25.0f;

	vector<glm::vec3> orbit;
	std::vector<float> temp1, temp2, temp3;
	int count = 10;

	while (count + 1 < points)
		count *= 10;
	count *= 100;

	for (auto i = 0; i < count; i++)
	{
		temp1.push_back(i);
		temp2.push_back(i);
		temp3.push_back(i);
	}

	std::random_shuffle(temp1.begin(), temp1.end());
	std::random_shuffle(temp2.begin(), temp2.end());
	std::random_shuffle(temp3.begin(), temp3.end());

	int i = 0;
	float x, y, z;
	while (orbit.size() < points && i < count)
	{

		x = (temp1[i] / (count / 10)) - 5.0;
		y = (temp2[i] / (count / 10)) - 5.0;
		z = (temp3[i] / (count / 10)) - 5.0;
		if (x*x + y*y + z*z <= 25.0 && x*x + y*y + z*z >= smaller_radius)
			orbit.push_back(glm::vec3(x, y, z));
		i++;
	}

	return orbit;
}

//vector<glm::vec3> generateSquare(long int points)
//{
//	vector<glm::vec3> square;
//	float x, y, z;
//
//	for (int i = 0; i < points; i++)
//	{
//		x = ((float)(rand() % 1001) - 500) / 100;
//		y = ((float)(rand() % 1001) - 500) / 100;
//		z = ((float)(rand() % 1001) - 500) / 100;
//		square.push_back(glm::vec3(x, y, z));
//	}
//
//	return square;
//}
//
//vector<glm::vec3> generateSphere(long int points)
//{
//	vector<glm::vec3> sphere;
//	float x, y, z;
//
//	for (int i = 0; i < points; i++)
//	{
//		x = ((float)(rand() % 1001) - 500) / 100;
//		y = ((float)(rand() % 1001) - 500) / 100;
//		z = ((float)(rand() % 1001) - 500) / 100;
//		if (x*x + y*y + z*z <= 25.0)
//			sphere.push_back(glm::vec3(x, y, z));
//	}
//
//	while (sphere.size() != points)
//	{
//		x = ((float)(rand() % 1001) - 500) / 100;
//		y = ((float)(rand() % 1001) - 500) / 100;
//		z = ((float)(rand() % 1001) - 500) / 100;
//		if (x*x + y*y + z*z <= 25.0)
//			sphere.push_back(glm::vec3(x, y, z));
//	}
//
//	return sphere;
//}
//
//vector<glm::vec3> generateOrbit(long int points, GLfloat smaller_radius)
//{
//	vector<glm::vec3> orbit;
//	float x, y, z;
//	if (smaller_radius > 25.0f)
//		smaller_radius = 25.0f;
//
//	for (int i = 0; i < points; i++)
//	{
//		x = ((float)(rand() % 1001) - 500) / 100;
//		y = ((float)(rand() % 1001) - 500) / 100;
//		z = ((float)(rand() % 1001) - 500) / 100;
//		if (x*x + y*y + z*z <= 25.0 && x*x + y*y + z*z >= smaller_radius)
//			orbit.push_back(glm::vec3(x, y, z));
//	}
//
//	while (orbit.size() != points)
//	{
//		x = ((float)(rand() % 1001) - 500) / 100;
//		y = ((float)(rand() % 1001) - 500) / 100;
//		z = ((float)(rand() % 1001) - 500) / 100;
//		if (x*x + y*y + z*z <= 25.0 && x*x + y*y + z*z >= smaller_radius)
//			orbit.push_back(glm::vec3(x, y, z));
//	}
//
//	return orbit;
//}

//prints progress of iterations 
void printProgress(int actual, int maximum)
{
	int percentage = actual * 100 / maximum;
	system("cls");
	cout << "[";
	for (int i = 0; i < percentage; i++)
		cout << "=";
	for (int i = 100 - percentage; i > 0; i--)
		cout << " ";
	cout << "]";
	cout << "[" << percentage << "%]" << endl;
}

void printResultsTable(double ** results, int size)
{
	cout << "============================================================================================================" << endl;
	for (int i = 0; i < size; i++)
	{
		cout.precision(6);
		cout << "\t\t\t" << results[i][0] << "\t\t";
		cout << results[i][1] << " ms\t\t";
		cout << results[i][2] << endl;
	}
	cout << "============================================================================================================" << endl;
}
