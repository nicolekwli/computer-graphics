#include "TexturePoint.h"
#include <iostream>

class CanvasPoint
{
  public:
    float x;
    float y;
    double depth;
    float brightness;
    TexturePoint texturePoint;

    CanvasPoint();

    CanvasPoint(float xPos, float yPos);

    CanvasPoint(float xPos, float yPos, float pointDepth);

    CanvasPoint(float xPos, float yPos, float pointDepth, float pointBrightness);

};

std::ostream& operator<<(std::ostream& os, const CanvasPoint& point);