#include "draw_scene.hpp"

/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{100.0};	 // Distance between origin and viewpoint

GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};

GLBI_Set_Of_Points frame{3};
IndexedMesh *sphere;
float rotation{0};

GLBI_Convex_2D_Shape cercle{3};
StandardMesh *cone;

void initScene()
{
	std::vector<float> points{0.0, 0.0, 0.0};
	somePoints.initSet(points, 1.0, 1.0, 1.0);

	//  case 10x10
	std::vector<float> baseCarre{
		0.0, 0.0, 0.0,
		10.0, 0.0, 0.0,
		10.0, 10.0, 0.0,
		0.0, 10.0, 0.0};

	ground.initShape(baseCarre);
	ground.changeNature(GL_TRIANGLE_FAN);

	// FRAME
	frame.changeNature(GL_LINES);

}

void drawScene()
{
	// Positionnement caméra en coordonnées sphériques
	float cam_x = dist_zoom * cos(angle_phy * M_PI / 180.f) * cos(angle_theta * M_PI / 180.f);
	float cam_y = dist_zoom * cos(angle_phy * M_PI / 180.f) * sin(angle_theta * M_PI / 180.f);
	float cam_z = dist_zoom * sin(angle_phy * M_PI / 180.f);

	// Création de la matrice lookAt avec STP3D::Matrix4D
	STP3D::Matrix4D viewMatrix = STP3D::Matrix4D::lookAt(
		STP3D::Vector3D(cam_x, cam_y, cam_z), // position caméra
		STP3D::Vector3D(0.f, 0.f, 0.f),		  // point regardé
		STP3D::Vector3D(0.f, 0.f, 1.f)		  // vecteur haut (Z vers le haut)
	);

	// Appliquer la matrice de vue au moteur
	myEngine.mvMatrixStack.loadTransformation(viewMatrix);
	myEngine.updateMvMatrix();

	glPointSize(10.0);

	// drawFrame();

	myEngine.setFlatColor(0.31, 0.459, 0.267);
	const int N = 10;
	const float CASE_SIZE = 10.0f;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			myEngine.mvMatrixStack.pushMatrix();
			// decalage des cases
			myEngine.mvMatrixStack.addTranslation({(i - N / 2) * CASE_SIZE,
												   (j - N / 2) * CASE_SIZE,
												   0.0f});
			myEngine.updateMvMatrix();

			// cases qui alternent entre deux verts
			if ((i + j) % 2 == 0)
			{
				myEngine.setFlatColor(0.31f, 0.459f, 0.267f); // vert clair
			}
			else
			{
				myEngine.setFlatColor(0.267, 0.412, 0.227); // vert foncé
			}
			ground.drawShape();

			myEngine.mvMatrixStack.popMatrix();
			myEngine.updateMvMatrix();
		}
	}
}