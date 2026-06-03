#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "../json.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};

extern float cam_x;// Position X de la caméra
extern float cam_y;// Position Y (hauteur)
extern float cam_z;// Position Z de la caméra
extern float cam_angle;// Angle de rotation

//circut du json
struct Circuit {
    int size_grid;
    std::vector<std::pair<int,int>> path;
};

/* OpenGL Engine */
extern GLBI_Engine myEngine;

void initScene(const std::string& jsonFile);

void drawScene();
