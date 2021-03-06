#include <glm/glm.hpp>
#include "DrawingWindow.h"
#include "CanvasPoint.h"
#include "CanvasTriangle.h"
#include "Colour.h"
#include "Helper.h"
#include "Rasterise.h"
#include "Camera.h"
#include "Raytracer.h"
#include <vector>


using namespace std;
using namespace glm;

void update();
void draw();
void handleEvent(SDL_Event event);

void cornellboxAnimate();
void logoFlyThrough();
void wipeScreen();
void logoOrbit();
void clearScreen();