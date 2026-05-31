#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "../draw_scene.hpp"

using namespace glbasimac;

// Définis dans draw_scene.cpp
extern GLBI_Engine myEngine;
extern Circuit circuit;

// Constantes rails
extern const float POS_X_RAIL1;
extern const float POS_X_RAIL2;
extern const float sr;
extern const float sx;
extern const float rr;

void initRail();
void drawRailDroit();
void drawRailCourbe();
void drawRails();