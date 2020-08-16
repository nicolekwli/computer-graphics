#pragma once

#include <glm/glm.hpp>
#include "Colour.h"
#include "Material.h"
#include "TexturePoint.h"
#include <string>

using namespace std;
using namespace glm;

// struct Mat{
//     string name;
//     vec3 diffuse; //kd -diffuse
//     vec3 ambient; //ka
//     vec3 specular; //ks
//     float highlight; //ns
//     float illum;
// };

class ModelTriangle
{
  public:
    glm::vec3 vertices[3];
    glm::vec3 normals[3];
    TexturePoint texturePoints[3];
    Colour colour;
    Material mat;
    string objname;

    ModelTriangle();
    ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
    ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour);
};

std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle);
