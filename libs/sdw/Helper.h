#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "DrawingWindow.h"
#include "TexturePoint.h"
#include "Colour.h"
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
TexturePoint getTexturePoint(CanvasPoint from, CanvasPoint to, CanvasPoint p);

// ----- Drawing -----
void drawLine(DrawingWindow window, CanvasPoint p1, CanvasPoint p2, Colour c);
void drawStrokedTriangle(DrawingWindow window, CanvasTriangle t);
void fillTriangle(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, float stepsTopLeft, float stepsTopRight, Colour c); 
void drawFilledTriangle(DrawingWindow window, Colour c);


void fillTexture(DrawingWindow window, vector<CanvasPoint> lineTopLeft, vector<CanvasPoint> lineTopRight, vector<vector<uint32_t>> pixels);

// this should take in triangle
void fillTextureTriangle(DrawingWindow window, vector<vector<uint32_t>> pixels, CanvasTriangle t);

// ----- Parsing -----
vector<vector<uint32_t>> readPPM(DrawingWindow window, string filename);