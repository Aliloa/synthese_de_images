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
bool phareOn = false;
bool flatMode = false;
bool lightsInitialized = false;
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

	// Position phare
	int idx = circuit.train_pos;
	int next = (idx + 1) % (int)circuit.path.size();
	float posX = circuit.path[idx].first * circuit.size_grid + circuit.size_grid / 2.f;
	float posY = circuit.path[idx].second * circuit.size_grid + circuit.size_grid / 2.f;
	float dx = circuit.path[next].first - circuit.path[idx].first;
	float dy = circuit.path[next].second - circuit.path[idx].second;
	float angle = atan2f(dy, dx);
	float phareX = posX + dx * 12.f + cosf(angle) * 5.f;
	float phareY = posY + dy * 12.f + sinf(angle) * 5.f;

	// Reset lumières si changement de mode jour/nuit
	static bool lastLightingMode = lightingMode;
	if (lastLightingMode != lightingMode)
	{
		lightsInitialized = false;
		if (!lightingMode)
			phareOn = false;
		lastLightingMode = lightingMode;
	}

	if (lightingMode)
	{
		// === NUIT ===
		glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!lightsInitialized)
		{
			// Lumière 0 : lune froide très faible
			myEngine.addALight(
					STP3D::Vector4D(0.f, 0.f, 1.f, 0.f),
					STP3D::Vector3D(0.12f, 0.12f, 0.2f));

			// Lumière 1 : phare du train (jaune doux, pas trop fort)
			myEngine.addALight(
					STP3D::Vector4D(phareX, phareY, 3.f, 1.f),
					phareOn
							? STP3D::Vector3D(3.f, 3.f, 2.f)
							: STP3D::Vector3D(0.f, 0.f, 0.f));

			// Lumière 2 : ambiance très faible depuis le bas
			myEngine.addALight(
					STP3D::Vector4D(0.f, 0.f, -1.f, 0.f),
					STP3D::Vector3D(0.05f, 0.05f, 0.08f));

			// Lumière 3 : latérale gauche
			myEngine.addALight(
					STP3D::Vector4D(1.f, 0.f, 0.f, 0.f),
					STP3D::Vector3D(0.04f, 0.04f, 0.07f));

			// Lumière 4 : latérale droite
			myEngine.addALight(
					STP3D::Vector4D(-1.f, 0.f, 0.f, 0.f),
					STP3D::Vector3D(0.04f, 0.04f, 0.07f));

			myEngine.setShininess(8.f);
			myEngine.setSpecularColor(STP3D::Vector3D(0.3f, 0.3f, 0.3f));
			myEngine.setAttenuationFactor(STP3D::Vector3D(1.f, 0.05f, 0.005f));
			lightsInitialized = true;
		}

		// Mise à jour phare chaque frame
		myEngine.setLightPosition(
				STP3D::Vector4D(phareX, phareY, 3.f, 1.f), 1);
		myEngine.setLightIntensity(
				phareOn
						? STP3D::Vector3D(3.f, 3.f, 2.f)
						: STP3D::Vector3D(0.f, 0.f, 0.f),
				1);

		// Sol en flat vert pur la nuit
		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
		drawGround();

		if (!flatMode)
		{
			myEngine.switchToPhongShading();
			glUniformMatrix4fv(
					glGetUniformLocation(myEngine.idShader[1], "viewMatrix"),
					1, GL_FALSE, viewMatrix.mat);
		}
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();

		drawRails();
		drawStation();
		drawRandomSapins();
		drawRandomShrooms();

		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
		drawLune();

		if (!flatMode)
		{
			myEngine.switchToPhongShading();
			glUniformMatrix4fv(
					glGetUniformLocation(myEngine.idShader[1], "viewMatrix"),
					1, GL_FALSE, viewMatrix.mat);
		}
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
	}
	else
	{
		// === JOUR ===
		glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!lightsInitialized)
		{
			// Lumière 0 : soleil zénith blanc, modéré
			myEngine.addALight(
					STP3D::Vector4D(0.f, 0.f, 1.f, 0.f),
					STP3D::Vector3D(0.7f, 0.7f, 0.7f));

			// Lumière 1 : fill bas (ciel réfléchi)
			myEngine.addALight(
					STP3D::Vector4D(0.f, 0.f, -1.f, 0.f),
					STP3D::Vector3D(0.25f, 0.25f, 0.25f));

			// Lumière 2 : fill avant
			myEngine.addALight(
					STP3D::Vector4D(0.f, 1.f, 0.f, 0.f),
					STP3D::Vector3D(0.2f, 0.2f, 0.2f));

			// Lumière 3 : fill arrière
			myEngine.addALight(
					STP3D::Vector4D(0.f, -1.f, 0.f, 0.f),
					STP3D::Vector3D(0.2f, 0.2f, 0.2f));

			// Lumière 4 : fill droite
			myEngine.addALight(
					STP3D::Vector4D(1.f, 0.f, 0.f, 0.f),
					STP3D::Vector3D(0.15f, 0.15f, 0.15f));

			// Lumière 5 : fill gauche
			myEngine.addALight(
					STP3D::Vector4D(-1.f, 0.f, 0.f, 0.f),
					STP3D::Vector3D(0.15f, 0.15f, 0.15f));

			myEngine.setShininess(4.f);
			myEngine.setSpecularColor(STP3D::Vector3D(1.f, 1.f, 1.f));
			myEngine.setAttenuationFactor(STP3D::Vector3D(1.f, 0.0f, 0.0f));
			lightsInitialized = true;
		}

		// Sol en flat vert sans ombres
		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
		drawGround();

		if (!flatMode)
		{
			myEngine.switchToPhongShading();
			glUniformMatrix4fv(
					glGetUniformLocation(myEngine.idShader[1], "viewMatrix"),
					1, GL_FALSE, viewMatrix.mat);
		}
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();

		drawRails();
		drawStation();
		drawRandomSapins();
		drawRandomShrooms();

		myEngine.switchToFlatShading();
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
		drawSoleil();

		if (!flatMode)
		{
			myEngine.switchToPhongShading();
			glUniformMatrix4fv(
					glGetUniformLocation(myEngine.idShader[1], "viewMatrix"),
					1, GL_FALSE, viewMatrix.mat);
		}
		myEngine.mvMatrixStack.loadTransformation(viewMatrix);
		myEngine.updateMvMatrix();
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