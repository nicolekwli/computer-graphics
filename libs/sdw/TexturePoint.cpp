#include "TexturePoint.h"

TexturePoint::TexturePoint()
{
}

TexturePoint::TexturePoint(float xPos, float yPos)
{
  x = xPos;
  y = yPos;
}

    // void print()
    // {
    // }


std::ostream& operator<<(std::ostream& os, const TexturePoint& point)
{
    os << "(" << point.x << ", " << point.y << ")" << std::endl;
    return os;
}