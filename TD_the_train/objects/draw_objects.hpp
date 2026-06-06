#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "../draw_scene.hpp"

using namespace glbasimac;

// Définis dans draw_scene.cpp
extern GLBI_Engine myEngine;

void initObjects();
void drawSapin();
void drawRandomSapins();
void drawKyle(float handAnimation);
void drawStan(float handAnimation);
void drawCartman();