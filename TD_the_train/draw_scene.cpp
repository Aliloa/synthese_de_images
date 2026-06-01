#include "draw_scene.hpp"
#include "objects/draw_rail.hpp"
#include "objects/draw_ground.hpp"
#include "objects/draw_objects.hpp"
#include <fstream>	// pour std::ifstream
#include <iostream> // pour les erreurs éventuelles

/// Camera parameters
float angle_theta{90.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint

GLBI_Engine myEngine;
Circuit circuit;

void loadCircuit(const std::string &filename)
{
	// Ouvrir et parser le fichier JSON
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Impossible d'ouvrir : " << filename << std::endl;
		return;
	}
	nlohmann::json j = nlohmann::json::parse(file);

	// Remplir la structure
	circuit.size_grid = j["size_grid"];

	circuit.path.clear();
	for (auto &point : j["path"])
	{
		circuit.path.push_back({point[0], point[1]});
	}
}

void initScene()
{
	loadCircuit("../TD_the_train/circuit.json");
	std::cout << "size_grid: " << circuit.size_grid << std::endl;
	std::cout << "path: " << std::endl;
	for (auto &point : circuit.path)
	{
		std::cout << "  " << point.first << ", " << point.second << std::endl;
	}

	initGround();
	initRail();
	initObjects();
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

	drawGround();

	drawRails();

	drawRandomSapins();
}