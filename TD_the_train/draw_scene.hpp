#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "../json.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR{0.1f};
static const float Z_FAR{500.f};

extern float cam_x;
extern float cam_y;
extern float cam_z;
extern float cam_angle;

struct Circuit
{
    int size_grid;
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> station;
    int train_pos;
};

/* OpenGL Engine */
extern GLBI_Engine myEngine;
extern Circuit circuit;

void initScene(const std::string &jsonFile);

void drawScene();
