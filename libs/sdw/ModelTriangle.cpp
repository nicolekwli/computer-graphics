#include "ModelTriangle.h"

ModelTriangle::ModelTriangle()
{
}

ModelTriangle::ModelTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Colour trigColour)
{
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;

    texturePoints[0]  = TexturePoint(-1,-1);
    texturePoints[1]  = TexturePoint(-1,-1);
    texturePoints[2]  = TexturePoint(-1,-1);
    
    colour = trigColour;
}


std::ostream& operator<<(std::ostream& os, const ModelTriangle& triangle)
{
    os << "(" << triangle.vertices[0].x << ", " << triangle.vertices[0].y << ", " << triangle.vertices[0].z << ")" << std::endl;
    os << "(" << triangle.vertices[1].x << ", " << triangle.vertices[1].y << ", " << triangle.vertices[1].z << ")" << std::endl;
    os << "(" << triangle.vertices[2].x << ", " << triangle.vertices[2].y << ", " << triangle.vertices[2].z << ")" << std::endl;
    os << "(" << triangle.texturePoints[0] << ")" << std::endl;
    os << "(" << triangle.texturePoints[1] << ")" << std::endl;
    os << "(" << triangle.texturePoints[2] << ")" << std::endl;
    os << std::endl;
    return os;
}
