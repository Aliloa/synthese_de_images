#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "../draw_scene.hpp"

using namespace glbasimac;

extern GLBI_Engine myEngine;
extern Circuit circuit;

void initStation();
void drawStation();