#pragma once

#include <glm/glm.hpp>
#include "Colour.h"
#include <string>

class ModelTriangle
{
  public:
    glm::vec3 vertices[3];
    Colour colour;

    ModelTriangle();

    ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour);
};

std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
