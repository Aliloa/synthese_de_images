#include "draw_scene.hpp"
#include "objects/draw_rail.hpp"
#include "objects/draw_ground.hpp"
#include "objects/draw_objects.hpp"
#include <fstream>	// pour std::ifstream
#include <iostream> // pour les erreurs éventuelles

/// Camera parameters
float cam_x = 0.f;
float cam_y = 5.f;
float cam_z = 5.f;
float cam_angle = 0.f;

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

void initScene(const std::string& jsonFile)
{
	loadCircuit(jsonFile); //le chemin du fichier est passé par l'utilisateur
	// loadCircuit("../TD_the_train/circuit.json");
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
	// calcul de la position de la camera
	float rad = cam_angle * M_PI / 180.f; // coinvertir l'angle en radians
	float dir_x = cosf(rad);
	float dir_y = sinf(rad);
	
	// matrice lookAt
	STP3D::Matrix4D viewMatrix = STP3D::Matrix4D::lookAt(
        STP3D::Vector3D(cam_x, cam_y, cam_z),               // position caméra
        STP3D::Vector3D(cam_x + dir_x, cam_y + dir_y, cam_z), // point regardé (devant)
        STP3D::Vector3D(0.f, 0.f, 1.f)                      // Z vers le haut (comme avant)
    );

	// Appliquer la matrice de vue au moteur
	myEngine.mvMatrixStack.loadTransformation(viewMatrix);
	myEngine.updateMvMatrix();

	drawGround();

	drawRails();

	drawRandomSapins();
}