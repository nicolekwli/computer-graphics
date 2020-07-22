#pragma once

#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "Colour.h"
#include "Camera.h"
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

CanvasPoint vertex3Dto2D(DrawingWindow window, vec3 vertex3D, Camera cam);
void modelToCanvasTri(DrawingWindow window, ModelTriangle mt, CanvasTriangle &ct, Camera cam);
vector<CanvasTriangle> clipping(DrawingWindow window, CanvasTriangle ct);
void createWireframe(DrawingWindow window, vector<ModelTriangle> triangles, Camera cam);
void rasterise(DrawingWindow window, vector<ModelTriangle> t, Camera cam, vector<vector<uint32_t>> pixels); // no texture