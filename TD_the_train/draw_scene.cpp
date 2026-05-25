#include "draw_scene.hpp"
#include <cmath>

/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{150.0};	 // Distance between origin and viewpoint

GLBI_Engine myEngine;

// Taille du terrain
static const int N = 10;
static const float CELL = 10.0f;

void initScene()
{
	glEnable(GL_DEPTH_TEST);
}

void drawTerrain()
{
	myEngine.setFlatColor(0.3f, 0.65f, 0.3f);

	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

void drawScene()
{
	// Projection
	Matrix4D proj = Matrix4D::perspective(60.0f, 1200.0f / 800.0f, 0.1f, 500.0f);
	myEngine.set3DProjection(60.0f, 1200.0f / 800.0f, 0.1f, 500.0f);

	// Caméra : position (50, 80, 150) regarde le centre du terrain (50, 0, 50)
	Matrix4D view = Matrix4D::lookAt(
		Vector3D(50.0f, 50.0f, 200.0f), // plus loin et moins haute
		Vector3D(50.0f, 0.0f, 50.0f),
		Vector3D(0.0f, 1.0f, 0.0f));
	myEngine.setViewMatrix(view);
	myEngine.updateMvMatrix();
	drawTerrain();
}
