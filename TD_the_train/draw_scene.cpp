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

// lumiere
bool lightingMode = false; // false = jour, true = nuit
bool lightingEnabled{false};

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

	initGround();
	initRail();
	initStation();
	initObjects();
	initTrain();

	// lumiere
	myEngine.switchToPhongShading();

	// Light init
	myEngine.addALight({0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
	myEngine.setLightPosition({5.0f, -5.0f, 8.0f, 0.0f}, 0); // soleil

	// fixe param
	myEngine.setShininess(8.0f);
	myEngine.setSpecularColor({0.2f, 0.2f, 0.15f});
	myEngine.setAttenuationFactor({1.0f, 0.05f, 0.01f});

	myEngine.switchToFlatShading();
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

	if (lightingMode)
	{
		glClearColor(0.03f, 0.03f, 0.12f, 1.0f); // nuit
	}
	else
	{
		glClearColor(0.5f, 0.7f, 0.9f, 1.0f); // jour
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myEngine.mvMatrixStack.loadTransformation(viewMatrix);
	myEngine.updateMvMatrix();

	if (lightingEnabled)
	{
		myEngine.switchToPhongShading();

		if (lightingMode)
		{
			// night
			myEngine.setLightPosition({0.0f, 0.0f, 100.0f, 0.0f}, 0);
			myEngine.setLightIntensity({0.5f, 0.5f, 0.5f}, 0);
		}
		else
		{
			// day
			myEngine.setLightPosition({5.0f, -5.0f, 8.0f, 0.0f}, 0);
			myEngine.setLightIntensity({1.4f, 1.2f, 0.9f}, 0);
		}
		// Soleil ou lune en flat
		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
		if (lightingMode)
			drawLune();
		else
			drawSoleil();

		myEngine.switchToPhongShading();
		glUniformMatrix4fv(glGetUniformLocation(myEngine.idShader[1], "viewMatrix"), 1, GL_FALSE, viewMatrix.mat);
	}
	else
	{
		myEngine.switchToFlatShading();
	}

	drawGround();
	drawRails();
	drawStation();
	drawRandomSapins();
	drawRandomShrooms();

	myEngine.mvMatrixStack.loadTransformation(viewMatrix);
	myEngine.updateMvMatrix();

	animate += 0.02f;
	armAnimationSlow = sin(animate * 3) * 0.8f;	 // pour que ca fasse des allé retour entre 0.8 et -0.8
	armAnimationFast = sin(animate * 10) * 0.8f; // pour que ca fasse des allé retour + rapide

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation({-19, 5, 0});
	myEngine.mvMatrixStack.addRotation(M_PI / 2, {0, 0, 1});
	myEngine.mvMatrixStack.addRotation(M_PI, {0, 0, 1});
	myEngine.mvMatrixStack.addHomothety({0.8, 0.8, 0.8});
	drawKyle(armAnimationSlow);
	myEngine.mvMatrixStack.addTranslation({5, 0, 0});
	drawStan(armAnimationFast);
	myEngine.mvMatrixStack.addTranslation({6, 0, 0});
	drawCartman(armAnimationSlow);
	myEngine.mvMatrixStack.popMatrix();

	drawTrain();
}