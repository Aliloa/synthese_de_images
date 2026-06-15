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
bool lightingMode = false;
static bool lightsInitialized = false;
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
	std::cout << "size_grid: " << circuit.size_grid << std::endl;
	for (auto &point : circuit.path)
		std::cout << "  " << point.first << ", " << point.second << std::endl;

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

	if (lightingMode)
	{
		glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myEngine.switchToPhongShading();
		glUniformMatrix4fv(glGetUniformLocation(myEngine.idShader[1], "viewMatrix"), 1, GL_FALSE, viewMatrix.mat);

		if (!lightsInitialized)
		{
			myEngine.addALight(
					STP3D::Vector4D(30.f, 30.f, 25.f, 0.f),
					STP3D::Vector3D(20.f, 20.f, 20.f));
			myEngine.setShininess(16.f);
			lightsInitialized = true;
		}

		int idx = circuit.train_pos;
		int next = (idx + 1) % (int)circuit.path.size();
		float posX = circuit.path[idx].first * circuit.size_grid + circuit.size_grid / 2.f;
		float posY = circuit.path[idx].second * circuit.size_grid + circuit.size_grid / 2.f;
		float dx = circuit.path[next].first - circuit.path[idx].first;
		float dy = circuit.path[next].second - circuit.path[idx].second;
		float angle = atan2f(dy, dx);

		myEngine.setLightPosition(STP3D::Vector4D(
																	posX + dx * 12.f + cosf(angle) * 5.f,
																	posY + dy * 12.f + sinf(angle) * 5.f,
																	3.f, 1.f),
															1);

		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();

		drawGround();
		drawRails();
		drawStation();
		drawRandomSapins();
		drawRandomShrooms();

		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
		drawLune();
		myEngine.switchToPhongShading();
		glUniformMatrix4fv(glGetUniformLocation(myEngine.idShader[1], "viewMatrix"), 1, GL_FALSE, viewMatrix.mat);
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
	}
	else
	{
		glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();

		drawGround();
		drawRails();
		drawStation();
		drawRandomSapins();
		drawRandomShrooms();
		drawSoleil();
	}

	animate += 0.02f;
	armAnimationSlow = sin(animate * 3) * 0.8f;
	armAnimationFast = sin(animate * 10) * 0.8f;

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