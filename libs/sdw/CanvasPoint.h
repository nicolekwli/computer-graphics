#pragma once

#include "TexturePoint.h"
#include "Colour.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
using glm::vec3;

class CanvasPoint
{
  public:
    float x;
    float y;
    double depth;
    float brightness;
    TexturePoint texturePoint;
    vec3 normal;
    Colour c;

    CanvasPoint();

    CanvasPoint(float xPos, float yPos);

    CanvasPoint(float xPos, float yPos, float pointDepth);

    CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness);

};

std::ostream& operator<<(std::ostream& os, const CanvasPoint& point);