#pragma once

#include "CanvasPoint.h"
#include "Colour.h"
#include <glm/glm.hpp>
#include <iostream>

using namespace glm;

using glm::vec3;

class CanvasTriangle
{
  public:
    CanvasPoint vertices[3];
    Colour colour;
    vec3 normal;

    CanvasTriangle();

    CanvasTriangle(CanvasPoint v0, CanvasPoint v1, CanvasPoint v2);

    CanvasTriangle(CanvasPoint v0, CanvasPoint v1, CanvasPoint v2, Colour c);

};

std::ostream& operator<<(std::ostream& os, const CanvasTriangle& triangle);
