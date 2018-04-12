#include "View.h"
using namespace std;

float camRotAngle = 1.0f;
int keysCounter = 0;			//means how many keys user holds at the moment
Key pressedKey;

View::View(std::vector<glm::vec3> &vert, std::vector<GLuint> &indi)
	:vertices(new std::vector<GLfloat>), indices(indi), pointsProgram(nullptr), trianglesProgram(nullptr)
{
	for (int i = 0; i < vert.size(); i++)
	{
		vertices->push_back(vert[i][0]);
		vertices->push_back(vert[i][1]);
		vertices->push_back(vert[i][2]);
	}

	try
	{
		if (glfwInit() != GL_TRUE)
			throw exception("GLFW initialization failed");
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
}

void View::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS && (keysCounter++) == 0)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_LEFT:
			pressedKey = Key::Left;
			break;
		case GLFW_KEY_RIGHT:
			pressedKey = Key::Right;
			break;
		case GLFW_KEY_UP:
			pressedKey = Key::Up;
			break;
		case GLFW_KEY_DOWN:
			pressedKey = Key::Down;
			break;
		case GLFW_KEY_KP_ADD:
			camRotAngle += 0.5f;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			if (camRotAngle >= 0.5f)
				camRotAngle -= 0.5f;
			break;
		}
	}
	else if (action == GLFW_RELEASE && (keysCounter--) == 1)
		pressedKey = Key::Nothing;
}

GLFWwindow* View::initialize()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ConvexHull3D", nullptr, nullptr);
	if (window == nullptr)
		throw exception("GLFW window not created");
	glfwMakeContextCurrent(window);

	glfwSetWindowUserPointer(window, this);

	auto func = [](GLFWwindow* w, int x, int y, int z, int l)
	{
		static_cast<View*>(glfwGetWindowUserPointer(w))->keyCallback(w, x, y, z, l);
	};
	glfwSetKeyCallback(window, func);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw exception("GLEW Initialization failed");
	glViewport(0, 0, WIDTH, HEIGHT);

	//mixes color from shader with color in color buffer what means background
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(POINT_SIZE);
	glLineWidth(LINE_WIDTH);
	return window;
}

glm::mat4 View::camRotation(glm::vec4 camPosition, glm::vec3 camUpVector)
{
	glm::mat4 camRotX;
	glm::mat4 camRotY;
	switch (pressedKey)
	{
	case Key::Left:
		camRotY = glm::rotate(camRotY, glm::radians(camRotAngle), camUpVector);
		break;
	case Key::Right:
		camRotY = glm::rotate(camRotY, glm::radians(-camRotAngle), camUpVector);
		break;
	case Key::Up:
		camRotX = glm::rotate(camRotX, glm::radians(-camRotAngle), cross(glm::vec3(camPosition), camUpVector));
		break;
	case Key::Down:
		camRotX = glm::rotate(camRotX, glm::radians(camRotAngle), cross(glm::vec3(camPosition), camUpVector));
		break;
	}
	return camRotX * camRotY;
}

void View::bindToVAO(GLuint VAO, GLuint VBO, GLuint EBO)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat), &(*vertices)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void View::drawElements(GLuint VAO, glm::mat4 view, glm::mat4 projection)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pointsProgram->Use();
	glBindVertexArray(VAO);
	glUniformMatrix4fv(glGetUniformLocation(pointsProgram->getProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(pointsProgram->getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glDrawArrays(GL_POINTS, 0, vertices->size() / 3);
	glBindVertexArray(0);

	trianglesProgram->Use();
	glBindVertexArray(VAO);
	glUniformMatrix4fv(glGetUniformLocation(trianglesProgram->getProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(trianglesProgram->getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glm::vec4 color = glm::vec4(0.2f, 0.4f, 0.6f, 0.5f);
	glUniform4fv(glGetUniformLocation(trianglesProgram->getProgramID(), "color"), 1, glm::value_ptr(color));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	color = glm::vec4(1.0, 1.0, 1.0, 0.2);
	glUniform4fv(glGetUniformLocation(trianglesProgram->getProgramID(), "color"), 1, glm::value_ptr(color));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
	glUseProgram(0);
}
void View::visualize()
{
	GLFWwindow* window = initialize();
	pointsProgram = new Shader("ConvexHull3D.vert", "Points.frag");
	trianglesProgram = new  Shader("ConvexHull3D.vert", "Triangles.frag");

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	bindToVAO(VAO, VBO, EBO);

	glm::vec3 camUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec4 camPosition = glm::vec4(0.0f, 0.0f, 17.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glm::mat4 camRot = camRotation(camPosition, camUpVector);
		camPosition = glm::vec4(camRot * camPosition);
		camUpVector = glm::mat3(camRot)*camUpVector;
		glm::mat4 view = glm::lookAt(glm::vec3(camPosition), glm::vec3(0.0f, 0.0f, 0.0f), camUpVector);
		glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		drawElements(VAO, view, projection);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
}

View::~View()
{
	delete pointsProgram;
	delete trianglesProgram;
}