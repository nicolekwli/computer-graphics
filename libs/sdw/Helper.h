#pragma once 

#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "DrawingWindow.h"
#include "TexturePoint.h"
#include "Material.h"
#include "Colour.h"
#include "Utils.h"
#include "Camera.h"
#include "Ppm.h"
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

// Maybe this should be in main
using namespace std;
using namespace glm;

using std::vector;
using glm::vec3;

uint32_t bitpackingColour(Colour c);

// ----- Interpolation -----
// Maybe this should return an int for convenience
vector<float> interpolation(float from, float to, int noOfVals);
vector<vec3> interpolation(vec3 from, vec3 to, int noOfVals);
vector<CanvasPoint> interpolation(CanvasPoint a, CanvasPoint b, float noOfVals);
void texPointCorrected(CanvasPoint a, CanvasPoint b, CanvasPoint &newP);


// ----- Drawing -----
void drawLine(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c);
void drawLineB(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c);
void drawLineWu(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c);
void drawStrokedTriangle(DrawingWindow window, CanvasTriangle t);
void fillTriangle(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, Colour c, int kind); 
void drawFilledTriangle(DrawingWindow window, Colour c, CanvasTriangle triangle, int kind, vec3 cameraPos);
void fillTexture(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, vector<vector<uint32_t>> pixels);
void fillTextureTriangle(DrawingWindow window, vector<vector<uint32_t>> pixels, CanvasTriangle t);

// ----- Parsing -----
PPM readPPM(DrawingWindow window, string filename);
void savePPM(DrawingWindow window, string filename);

// need to call mtl before obj
vector<ModelTriangle> readOBJ(string filename, vector<Colour> colours, PPM ppm, float rescale);
vector<ModelTriangle> readOBJAlt(string filename, vector<Material> mtls, PPM ppm, float rescale);
vector<Colour> readMTL(string filename);
vector<Material> readMTLAlt(string filename);

// anti aliasing
void SSAA(DrawingWindow window);