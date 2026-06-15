#include "draw_scene.hpp"
#include "objects/draw_rail.hpp"
#include "objects/draw_ground.hpp"
#include "objects/draw_objects.hpp"
#include "objects/draw_station.hpp"
#include "objects/draw_train.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION

float cam_x = 0.f;
float cam_y = 5.f;
float cam_z = 5.f;
float cam_angle = 0.f;

GLBI_Engine myEngine;
Circuit circuit;

void loadCircuit(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Impossible d'ouvrir : " << filename << std::endl;
		return;
	}
	nlohmann::json j = nlohmann::json::parse(file);

	circuit.size_grid = j["size_grid"];
	circuit.path.clear();
	for (auto &point : j["path"])
	{
		circuit.path.push_back({point[0], point[1]});
	}
	circuit.station = {j["station"][0], j["station"][1]};
	circuit.train_pos = j["train_pos"];
}

void initScene(const std::string &jsonFile)
{
	loadCircuit(jsonFile);
	// std::cout << "size_grid: " << circuit.size_grid << std::endl;
	// for (auto &point : circuit.path)
	// 	std::cout << "  " << point.first << ", " << point.second << std::endl;

	initGround();
	initRail();
	initStation();
	initObjects();
	initTrain();
}

float animate = 0.0f;
float armAnimationSlow = 0.0f;
float armAnimationFast = 0.0f;

void drawScene()
{
	float rad = cam_angle * M_PI / 180.f;
	float dir_x = cosf(rad);
	float dir_y = sinf(rad);

	STP3D::Matrix4D viewMatrix = STP3D::Matrix4D::lookAt(
		STP3D::Vector3D(cam_x, cam_y, cam_z),
		STP3D::Vector3D(cam_x + dir_x, cam_y + dir_y, cam_z),
		STP3D::Vector3D(0.f, 0.f, 1.f));

	myEngine.mvMatrixStack.loadTransformation(viewMatrix);
	myEngine.updateMvMatrix();

	drawGround();
	drawRails();
	drawStation();
	drawRandomSapins();
	drawRandomShrooms();

	animate += 0.02f;
	armAnimationSlow = sin(animate*3) * 0.8f; // pour que ca fasse des allé retour entre 0.8 et -0.8
	armAnimationFast = sin(animate*10) * 0.8f;	// pour que ca fasse des allé retour + rapide

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation({-19, 5, 0});
	myEngine.mvMatrixStack.addRotation(M_PI/2, {0, 0, 1});
	myEngine.mvMatrixStack.addRotation(M_PI, {0, 0, 1});
	myEngine.mvMatrixStack.addHomothety({0.8, 0.8, 0.8});
	drawKyle(armAnimationSlow);
	// myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation({5, 0, 0});
	drawStan(armAnimationFast);
	myEngine.mvMatrixStack.addTranslation({6, 0, 0});
	drawCartman(armAnimationSlow);
	myEngine.mvMatrixStack.popMatrix();
	drawTrain();
}