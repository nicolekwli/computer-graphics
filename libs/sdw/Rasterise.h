#pragma once

#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "Colour.h"
#include "DrawingWindow.h"
#include "Helper.h"
#include "Utils.h"
#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>

using namespace std;
using namespace glm;

using std::vector;
using glm::vec3;

CanvasPoint vertex3Dto2D(vec3 vertex3D);
void modelToCanvasTri(ModelTriangle mt, CanvasTriangle &ct);
void createWireframe(DrawingWindow window, vector<ModelTriangle> triangles);
void rasterise(DrawingWindow window, vector<ModelTriangle> t);