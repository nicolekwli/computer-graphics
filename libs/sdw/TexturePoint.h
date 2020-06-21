#pragma once

#include <iostream>

class TexturePoint
{
  public:
    float x;
    float y;

    TexturePoint();

    TexturePoint(float xPos, float yPos);

    //void print();
};

std::ostream& operator<<(std::ostream& os, const TexturePoint& point);
