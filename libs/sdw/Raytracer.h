#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "DrawingWindow.h"
#include "TexturePoint.h"
#include "Camera.h"
#include "Colour.h"
#include "Utils.h"
#include "Helper.h"
#include "RayTriangleIntersection.h"
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

using namespace std;
using namespace glm;

using std::vector;
using glm::vec3;

#define pi 3.14159f

void drawFilledTriangleRay(DrawingWindow window, vector<ModelTriangle> t, Camera cam);
void raytracingLighting(DrawingWindow window, vector<ModelTriangle> triangles, Camera cam);