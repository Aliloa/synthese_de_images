#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "draw_scene.hpp"
#include "tools/shaders.hpp"
#include <iostream>

using namespace glbasimac;
using namespace STP3D;

extern bool lightingMode;
static bool mousePressed = false;
static double lastMouseX = 0.0;
static double lastMouseY = 0.0;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "The train";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int width, int height)
{
	aspectRatio = width / (float)height;

	glViewport(0, 0, width, height);
	std::cerr << "Setting 3D projection" << std::endl;
	// TO DO EX01 part 2
	myEngine.set3DProjection(60.0f, aspectRatio, 0.1f, 500.0f);
}

void onKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
	int is_pressed = (action == GLFW_PRESS);
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_L:
		if (is_pressed)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case GLFW_KEY_P:
		if (is_pressed)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	default:
		break;
	case GLFW_KEY_F:
		if (is_pressed)
			lightingMode = !lightingMode;
		break;
	}
}

// Remplace ton onMouseButton existant
void onMouseButton(GLFWwindow *window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mousePressed = true;
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		}
		else if (action == GLFW_RELEASE)
		{
			mousePressed = false;
		}
	}
}

// Nouveau callback à ajouter
void onMouseMove(GLFWwindow * /*window*/, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
		return;
	}

	double dx = xpos - lastMouseX;
	lastMouseX = xpos;
	lastMouseY = ypos;

	cam_angle -= (float)dx * 0.2f; //- sinon c'est inversé
}

void processMovement(GLFWwindow *window, float dt)
{
	float speed = 10.f * dt;
	float rad = cam_angle * M_PI / 180.f;
	float fx = cosf(rad); // forward X
	float fy = sinf(rad); // forward Y
	float sx = -fy;				// strafe X (perpendiculaire)
	float sy = fx;				// strafe Y

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam_x += fx * speed;
		cam_y += fy * speed;
	} // Z azerty
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam_x -= fx * speed;
		cam_y -= fy * speed;
	} // S azerty
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam_x += sx * speed;
		cam_y += sy * speed;
	} // Q azerty
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam_x -= sx * speed;
		cam_y -= sy * speed;
	} // D azerty
}

// Ajouter aussi le scroll pour le zoom
// void onScroll(GLFWwindow * /*window*/, double /*xoffset*/, double yoffset)
// {
// 	dist_zoom -= yoffset * 1.5f;
// 	if (dist_zoom < 1.0f)
// 		dist_zoom = 1.0f;
// }
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "erreur : donnez un fichier JSON" << std::endl;
		std::cerr << "usage : " << argv[0] << " scene.json" << std::endl;
		return -1;
	}

	std::string jsonFile = argv[1];
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
	glfwSetMouseButtonCallback(window, onMouseButton);

	glfwSetCursorPosCallback(window, onMouseMove);
	// glfwSetScrollCallback(window, onScroll);

	std::cout << "Engine init" << std::endl;
	// TO DO EX01 part 2

	myEngine.mode2D = false;
	myEngine.initGL();
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	CHECK_GL;

	initScene(jsonFile);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	double elapsedTime{0.0};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		processMovement(window, (float)elapsedTime);

		/* Render begins here */

		// TO DO EX01 part 2
		myEngine.set3DProjection(60.0f, aspectRatio, 0.1f, 500.0f);
		// TO DO EX01 part 3

		drawScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		elapsedTime = glfwGetTime() - startTime;
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
