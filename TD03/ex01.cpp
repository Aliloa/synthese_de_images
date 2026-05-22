#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "tools/shaders.hpp"
#include <iostream>
#include "glbasimac/glbi_convex_2D_shape.hpp"
using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "TD03 Ex01";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
static const float GL_VIEW_SIZE = 100.0f;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points thePoints;
GLBI_Convex_2D_Shape carre;
GLBI_Convex_2D_Shape triangle;
GLBI_Convex_2D_Shape cercle;
GLBI_Convex_2D_Shape Trapeze;
GLBI_Convex_2D_Shape rectangle;

std::vector<float> origine = {0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f};
std::vector<float> colors = {1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f};

/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int width, int height)
{
	aspectRatio = width / (float)height;

	glViewport(0, 0, width, height);
	if (aspectRatio > 1)
	{
		myEngine.set2DProjection(-GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio,
														 -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
	}
	else
	{
		myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
														 -GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio);
	}
}

void onKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		{
			thePoints.changeNature(GL_LINE_STRIP);
		}
	}
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;

		glfwGetWindowSize(window, &width, &height);
		float cordx = (xpos - width / 2) * GL_VIEW_SIZE / height;
		float cordy = -(ypos - height / 2) * GL_VIEW_SIZE / height;
		std::vector<float> coord = {cordx, cordy};
		std::vector<float> colors = {1.f, 1.f, 1.f};
		thePoints.addAPoint(coord.data(), colors.data());
	}
}
void initScene()
{
	std::vector<float> pointsCarre = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f};

	carre.initShape(pointsCarre);

	std::vector<float> pointscercle;
	const int N = 50;

	pointscercle.push_back(0.0f);
	pointscercle.push_back(0.0f);

	for (int i = 0; i <= N; i++)
	{
		float angle = 2.0f * M_PI * i / N;
		float x = cos(angle);
		float y = sin(angle);

		pointscercle.push_back(x);
		pointscercle.push_back(y);
	}

	cercle.initShape(pointscercle);
	cercle.changeNature(GL_TRIANGLE_FAN);

	std::vector<float> pointsTrapeze = {
			60.0f,
			-10.0f,
			0.0f,
			-20.0f,
			0.0f,
			20.0f,
			60.0f,
			10.0f,
	};

	Trapeze.initShape(pointsTrapeze);
	Trapeze.changeNature(GL_TRIANGLE_FAN);

	std::vector<float> pointsrectangle = {
			-3.0f,
			5.0f,
			48.0f,
			3.0f,
			48.0f,
			-3.0f,
			2.0f,
			-5.0f,
	};

	rectangle.initShape(pointsrectangle);
	rectangle.changeNature(GL_TRIANGLE_FAN);
}

void drawFirstArm()
{

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addHomothety(20);
	myEngine.updateMvMatrix();
	cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(Vector3D(60.0f, 0.0f, 0.0f));
	myEngine.mvMatrixStack.addHomothety(10);
	myEngine.updateMvMatrix();
	cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.updateMvMatrix();
	Trapeze.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}
void drawRoundedSquare()
{
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.updateMvMatrix();
	carre.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}
void drawSecondArm()
{
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.updateMvMatrix();
	rectangle.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}

void renderScene()
{
	myEngine.setFlatColor(1.0f, 1.0f, 1.0f);
	// drawFirstArm();
	drawRoundedSquare();
	drawSecondArm();
}

int main(int /*argc*/, char ** /*argv*/)
{
	/* GLFW initialisation */
	GLFWwindow *window;
	if (!glfwInit())
		return -1;

	/* Try to uncomment this for MAC OS if it did not work */
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout << "Loading GL extension" << std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	std::cout << "Engine init" << std::endl;
	myEngine.initGL();
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	CHECK_GL;

	initScene();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render here */
		glClearColor(0.2f, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		renderScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		while (elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
			elapsedTime = glfwGetTime() - startTime;
		}
	}

	glfwTerminate();
	return 0;
}
